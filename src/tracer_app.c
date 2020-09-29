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
    TraceResult trace_result;
} app;

void tracer_app_init(void) {
    app.child_proc = 0;
    app.source_id = 0;
}

void tracer_app_quit(void) {
    tracer_app_kill_child_proc();
}

void tracer_app_kill_child_proc(void) {
    if (app.child_proc > 0) {
        kill(app.child_proc, SIGKILL);
        waitpid(app.child_proc, NULL, 0);
        app.child_proc = 0;
        if (app.source_id)
            g_source_remove(app.source_id);
        app.source_id = 0;
    }
}

static void tracer_app_log_syscall(void) {
    tracer_gui_log_syscall(&app.trace_result);
}

static void tracer_app_finish_trace(void) {
    tracer_gui_on_trace_finish();
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
static gboolean wait_syscall_source_func(gpointer data) {
    if (app.child_proc <= 0) {
        app.source_id = 0;
        return G_SOURCE_REMOVE;
    }

    int status;
    if (waitpid(app.child_proc, &status, WNOHANG) == 0) {
        return G_SOURCE_CONTINUE;
    }
    if (STOPPED_BY_SYSCALL(status)) {
        struct ptrace_syscall_info sc_info;
        ptrace(PTRACE_GET_SYSCALL_INFO, app.child_proc, sizeof(sc_info), &sc_info);
        switch (sc_info.op) {
            case PTRACE_SYSCALL_INFO_ENTRY:
                app.trace_result.has_retval = FALSE;
                app.trace_result.sysno = sc_info.entry.nr;
                for (int i = 0; i < 6; ++i)
                    app.trace_result.args[i] = sc_info.entry.args[i];
                ptrace(PTRACE_SYSCALL, app.child_proc, 0, 0);
                return G_SOURCE_CONTINUE;
            case PTRACE_SYSCALL_INFO_EXIT:
                app.trace_result.has_retval = TRUE;
                app.trace_result.retval = sc_info.exit.rval;
                tracer_app_log_syscall();
                if (app.continuous) {
                    ptrace(PTRACE_SYSCALL, app.child_proc, 0, 0);
                    return G_SOURCE_CONTINUE;
                } else {
                    app.source_id = 0;
                    return G_SOURCE_REMOVE;
                }
            default:
                fprintf(stderr, "Error: unexpected ptrace result\n");
                exit(EXIT_FAILURE);
        }
    }
    if (WIFEXITED(status)) {
        tracer_app_log_syscall();
        app.child_proc = 0;
        tracer_app_finish_trace();
        app.source_id = 0;
        return G_SOURCE_REMOVE;
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