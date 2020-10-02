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
    GThreadPool *thread_pool;
    GAsyncQueue *trace_result_queue;
};

typedef struct TraceParams {
    gboolean continuous;
    gchar **args;
} TraceParams;


static void tracer_queue_syscall(Tracer *tracer, SyscallInfo *syscall) {
    TraceResult *result = g_malloc(sizeof(TraceResult));
    *result = (TraceResult){type: TRACEE_SYSCALL, syscall: *syscall};
    g_async_queue_push(tracer->trace_result_queue, result);
}

static void tracer_queue_exit(Tracer *tracer, int status) {
    TraceResult *result = g_malloc(sizeof(TraceResult));
    *result = (TraceResult){type: TRACEE_EXIT, exit_status: status};
    g_async_queue_push(tracer->trace_result_queue, result);
}

static void tracer_queue_term(Tracer *tracer, int signal) {
    TraceResult *result = g_malloc(sizeof(TraceResult));
    *result = (TraceResult){type: TRACEE_TERMINATED, term_signal: signal};
    g_async_queue_push(tracer->trace_result_queue, result);
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
static void worker_thread(gpointer data, gpointer user_data) {
    TraceParams *params = data;
    Tracer *tracer = user_data;

    pid_t child = start_tracee(params->args[0], params->args);

    if (child > 0) {
        int status;
        SyscallInfo syscall;

        ptrace(PTRACE_SYSCALL, child, 0, 0);
        while (TRUE) {
            if (waitpid(child, &status, WNOHANG) == 0) {
                continue;
            }
            if (WIFEXITED(status)) {
                tracer_queue_syscall(tracer, &syscall);
                tracer_queue_exit(tracer, WEXITSTATUS(status));
                break;
            } else if (WIFSIGNALED(status)) {
                tracer_queue_term(tracer, WTERMSIG(status));
                break;
            } else if (STOPPED_BY_SYSCALL(status)) {
                struct ptrace_syscall_info sc_info;
                ptrace(PTRACE_GET_SYSCALL_INFO, child, sizeof(sc_info), &sc_info);
                switch (sc_info.op) {
                    case PTRACE_SYSCALL_INFO_ENTRY:
                        syscall.has_retval = FALSE;
                        syscall.number = sc_info.entry.nr;
                        for (int i = 0; i < 6; ++i)
                            syscall.args[i] = sc_info.entry.args[i];
                        ptrace(PTRACE_SYSCALL, child, 0, 0);
                        continue;
                    case PTRACE_SYSCALL_INFO_EXIT:
                        syscall.retval = sc_info.exit.rval;
                        syscall.has_retval = TRUE;
                        tracer_queue_syscall(tracer, &syscall);
                        break;
                    case PTRACE_SYSCALL_INFO_SECCOMP:
                        syscall.number = sc_info.seccomp.nr;
                        for (int i = 0; i < 6; ++i)
                            syscall.args[i] = sc_info.seccomp.args[i];
                        syscall.retval = sc_info.seccomp.ret_data;
                        syscall.has_retval = TRUE;
                        tracer_queue_syscall(tracer, &syscall);
                        break;
                }
            } else {
                ptrace(PTRACE_SYSCALL, child, 0, 0);
                continue;
            }

            ptrace(PTRACE_SYSCALL, child, 0, 0);
        }
    }

    g_strfreev(params->args);
    g_free(params);
}


Tracer *tracer_new() {
    Tracer *tracer = g_malloc(sizeof(Tracer));
    tracer->thread_pool = g_thread_pool_new(worker_thread, tracer, 1, TRUE, NULL);
    tracer->trace_result_queue = g_async_queue_new_full(g_free);
    return tracer;
}

void tracer_free(Tracer *tracer) {
    tracer_kill_child_proc(tracer);
    g_thread_pool_free(tracer->thread_pool, TRUE, TRUE);
    g_async_queue_unref(tracer->trace_result_queue);
    g_free(tracer);
}

void tracer_kill_child_proc(Tracer *tracer) {
    
}

TraceResult *tracer_pop_queued_result(Tracer *tracer) {
    return g_async_queue_try_pop(tracer->trace_result_queue);
}

gboolean tracer_start_trace_async(Tracer *tracer, gchar **args, gboolean continuous) {
    TraceParams *params = g_malloc(sizeof(TraceParams));
    *params = (TraceParams) {continuous: continuous, args: g_strdupv(args)};
    return g_thread_pool_push(tracer->thread_pool, params, NULL);
}

void tracer_trace_next(Tracer *tracer) {
    
}