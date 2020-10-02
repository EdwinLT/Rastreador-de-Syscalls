#include "tracer.h"
#include "tracer_gui.h"

#include <sys/ptrace.h>
#include <linux/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>

#include <gtk/gtk.h>

struct Tracer {
    pid_t child_proc;
    guint source_id;
    gboolean continuous;
    SyscallInfo syscall_buffer;
    GQueue trace_queue;
};

static void tracer_queue_syscall(Tracer *tracer) {
    TraceResult *result = g_malloc(sizeof(TraceResult));
    *result = (TraceResult){type: TRACEE_SYSCALL, syscall: tracer->syscall_buffer};
    g_queue_push_tail(&tracer->trace_queue, result);
}

static void tracer_queue_exit(Tracer *tracer, int status) {
    TraceResult *result = g_malloc(sizeof(TraceResult));
    *result = (TraceResult){type: TRACEE_EXIT, exit_status: status};
    g_queue_push_tail(&tracer->trace_queue, result);
}

static void tracer_queue_term(Tracer *tracer, int signal) {
    TraceResult *result = g_malloc(sizeof(TraceResult));
    *result = (TraceResult){type: TRACEE_TERMINATED, term_signal: signal};
    g_queue_push_tail(&tracer->trace_queue, result);
}


Tracer *tracer_new() {
    Tracer *tracer = g_malloc(sizeof(Tracer));
    tracer->child_proc = 0;
    tracer->source_id = 0;
    g_queue_init(&tracer->trace_queue);
    return tracer;
}

void tracer_free(Tracer *tracer) {
    tracer_kill_child_proc(tracer);
    g_queue_free_full(&tracer->trace_queue, g_free);
    g_free(tracer);
}

void tracer_kill_child_proc(Tracer *tracer) {
    if (tracer->child_proc <= 0)
        return;
    if (kill(tracer->child_proc, SIGKILL) == 0) {
        while (TRUE) {
            int status;
            waitpid(tracer->child_proc, &status, 0);
            if (WIFSIGNALED(status)) {
                tracer_queue_term(tracer, WTERMSIG(status));
                tracer->child_proc = 0;
                if (tracer->source_id)
                    g_source_remove(tracer->source_id);
                tracer->source_id = 0;
                break;
            }
        }
    }
}

TraceResult *tracer_pop_queued_result(Tracer *tracer) {
    return (TraceResult*) g_queue_pop_head(&tracer->trace_queue);
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
#define STOP_WAITING(tracer) {tracer->source_id = 0; return G_SOURCE_REMOVE;}

static gboolean tracer_wait_source_func(gpointer data) {
    Tracer *tracer = (Tracer*) data;
    if (tracer->child_proc <= 0) STOP_WAITING(tracer)

    int status;
    if (waitpid(tracer->child_proc, &status, WNOHANG) == 0) {
        return G_SOURCE_CONTINUE;
    }
    if (WIFEXITED(status)) {
        tracer_queue_syscall(tracer);
        tracer_queue_exit(tracer, WEXITSTATUS(status));
        tracer->child_proc = 0;
        STOP_WAITING(tracer)
    } else if (WIFSIGNALED(status)) {
        tracer_queue_term(tracer, WTERMSIG(status));
        tracer->child_proc = 0;
        STOP_WAITING(tracer)
    } else if (STOPPED_BY_SYSCALL(status)) {
        struct ptrace_syscall_info sc_info;
        ptrace(PTRACE_GET_SYSCALL_INFO, tracer->child_proc, sizeof(sc_info), &sc_info);
        switch (sc_info.op) {
            case PTRACE_SYSCALL_INFO_NONE:
                ptrace(PTRACE_SYSCALL, tracer->child_proc, 0, 0);
                return G_SOURCE_CONTINUE;
            case PTRACE_SYSCALL_INFO_ENTRY:
                tracer->syscall_buffer.has_retval = FALSE;
                tracer->syscall_buffer.number = sc_info.entry.nr;
                for (int i = 0; i < 6; ++i)
                    tracer->syscall_buffer.args[i] = sc_info.entry.args[i];
                ptrace(PTRACE_SYSCALL, tracer->child_proc, 0, 0);
                return G_SOURCE_CONTINUE;
            case PTRACE_SYSCALL_INFO_EXIT:
                tracer->syscall_buffer.retval = sc_info.exit.rval;
                tracer->syscall_buffer.has_retval = TRUE;
                break;
            case PTRACE_SYSCALL_INFO_SECCOMP:
                tracer->syscall_buffer.number = sc_info.seccomp.nr;
                for (int i = 0; i < 6; ++i)
                    tracer->syscall_buffer.args[i] = sc_info.seccomp.args[i];
                tracer->syscall_buffer.retval = sc_info.seccomp.ret_data;
                tracer->syscall_buffer.has_retval = TRUE;
                break;
        }
        tracer_queue_syscall(tracer);
        if (!tracer->continuous) STOP_WAITING(tracer)
    }
    ptrace(PTRACE_SYSCALL, tracer->child_proc, 0, 0);
    return G_SOURCE_CONTINUE;
}

gboolean tracer_start_trace(Tracer *tracer, gchar **args, gboolean continuous) {
    if (tracer->child_proc > 0) return FALSE;
    pid_t child = start_tracee(args[0], args);

    if (child > 0) {
        tracer->child_proc = child;
        tracer->continuous = continuous;
        g_queue_clear_full(&tracer->trace_queue, g_free);
        ptrace(PTRACE_SYSCALL, child, 0, 0);
        tracer->source_id = g_idle_add(tracer_wait_source_func, tracer);
        return TRUE;
    } else return FALSE;
}

void tracer_trace_next(Tracer *tracer) {
    if (tracer->child_proc > 0 && tracer->source_id == 0) {
        ptrace(PTRACE_SYSCALL, tracer->child_proc, 0, 0);
        tracer->source_id = g_idle_add(tracer_wait_source_func, tracer);
    }
}