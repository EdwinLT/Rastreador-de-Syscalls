#include "tracer_app.h"
#include "tracer_gui.h"

#include <sys/ptrace.h>
#include <linux/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>

#include <gtk/gtk.h>

static struct {
    pid_t child_proc;
    guint source_id;
    gboolean continuous;
    SyscallInfo syscall_buffer;
    GQueue trace_queue;
} app;

static void tracer_app_queue_syscall(void) {
    TraceResult *result = g_malloc(sizeof(TraceResult));
    *result = (TraceResult){type: TRACEE_SYSCALL, syscall: app.syscall_buffer};
    g_queue_push_tail(&app.trace_queue, result);
}

static void tracer_app_queue_exit(int status) {
    TraceResult *result = g_malloc(sizeof(TraceResult));
    *result = (TraceResult){type: TRACEE_EXIT, exit_status: status};
    g_queue_push_tail(&app.trace_queue, result);
}

static void tracer_app_queue_term(int signal) {
    TraceResult *result = g_malloc(sizeof(TraceResult));
    *result = (TraceResult){type: TRACEE_TERMINATED, term_signal: signal};
    g_queue_push_tail(&app.trace_queue, result);
}


void tracer_app_init(void) {
    app.child_proc = 0;
    app.source_id = 0;
    g_queue_init(&app.trace_queue);
}

void tracer_app_quit(void) {
    tracer_app_kill_child_proc();
    g_queue_free_full(&app.trace_queue, g_free);
}

void tracer_app_kill_child_proc(void) {
    if (app.child_proc <= 0)
        return;
    if (kill(app.child_proc, SIGKILL) == 0) {
        while (TRUE) {
            int status;
            waitpid(app.child_proc, &status, 0);
            if (WIFSIGNALED(status)) {
                tracer_app_queue_term(WTERMSIG(status));
                app.child_proc = 0;
                if (app.source_id)
                    g_source_remove(app.source_id);
                app.source_id = 0;
                break;
            }
        }
    }
}

TraceResult *tracer_app_pop_queued_result(void) {
    return (TraceResult*) g_queue_pop_head(&app.trace_queue);
}


static pid_t start_tracee(const char *pathname, char *const *args) {
    pid_t child = fork();

    if (child == 0) {
        ptrace(PTRACE_TRACEME);
        kill(getpid(), SIGSTOP);
        exit(execvp(pathname, args));
    } else {
        if (child > 0) {
            int status;
            waitpid(child, &status, 0);
            ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
        }
        return child;
    }
}

#define STOPPED_BY_SYSCALL(status) (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80)
#define REMOVE_GSOURCE {app.source_id = 0; return G_SOURCE_REMOVE;}
static gboolean wait_syscall_source_func(gpointer data) {
    if (app.child_proc <= 0) REMOVE_GSOURCE

    int status;
    if (waitpid(app.child_proc, &status, WNOHANG) == 0) {
        return G_SOURCE_CONTINUE;
    }

    if (STOPPED_BY_SYSCALL(status)) {
        struct ptrace_syscall_info sc_info;
        ptrace(PTRACE_GET_SYSCALL_INFO, app.child_proc, sizeof(sc_info), &sc_info);
        switch (sc_info.op) {
            case PTRACE_SYSCALL_INFO_ENTRY:
                app.syscall_buffer.has_retval = FALSE;
                app.syscall_buffer.number = sc_info.entry.nr;
                for (int i = 0; i < 6; ++i)
                    app.syscall_buffer.args[i] = sc_info.entry.args[i];
                ptrace(PTRACE_SYSCALL, app.child_proc, 0, 0);
                return G_SOURCE_CONTINUE;

            case PTRACE_SYSCALL_INFO_EXIT:
                app.syscall_buffer.retval = sc_info.exit.rval;
                app.syscall_buffer.has_retval = TRUE;
                tracer_app_queue_syscall();
                if (!app.continuous) REMOVE_GSOURCE
                break;

            case PTRACE_SYSCALL_INFO_SECCOMP:
                app.syscall_buffer.number = sc_info.seccomp.nr;
                for (int i = 0; i < 6; ++i)
                    app.syscall_buffer.args[i] = sc_info.seccomp.args[i];
                app.syscall_buffer.retval = sc_info.seccomp.ret_data;
                app.syscall_buffer.has_retval = TRUE;
                tracer_app_queue_syscall();
                if (!app.continuous) REMOVE_GSOURCE
        }
    } else if (WIFEXITED(status)) {
        tracer_app_queue_syscall();
        tracer_app_queue_exit(WEXITSTATUS(status));
        app.child_proc = 0;
        REMOVE_GSOURCE
    } else if (WIFSIGNALED(status)) {
        tracer_app_queue_term(WTERMSIG(status));
        app.child_proc = 0;
        REMOVE_GSOURCE
    }

    ptrace(PTRACE_SYSCALL, app.child_proc, 0, 0);
    return G_SOURCE_CONTINUE;
}

gboolean tracer_app_start_trace(gchar **args, gboolean continuous) {
    if (app.child_proc > 0) return FALSE;
    pid_t child = start_tracee(args[0], args);

    if (child > 0) {
        app.child_proc = child;
        app.continuous = continuous;
        g_queue_clear_full(&app.trace_queue, g_free);
        ptrace(PTRACE_SYSCALL, child, 0, 0);
        app.source_id = g_idle_add(wait_syscall_source_func, NULL);
        return TRUE;
    } else return FALSE;
}

void tracer_app_trace_next(void) {
    if (app.child_proc > 0 && app.source_id == 0) {
        ptrace(PTRACE_SYSCALL, app.child_proc, 0, 0);
        app.source_id = g_idle_add(wait_syscall_source_func, NULL);
    }
}