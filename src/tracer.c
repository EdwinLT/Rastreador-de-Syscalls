#include "tracer.h"
#include "tracer_gui.h"

#include <sys/ptrace.h>
#include <linux/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>

#include <gtk/gtk.h>

typedef enum {
    PAUSE_FLAG,
    CONTINUE_FLAG,
    KILL_FLAG,
} TracerFlag;

struct Tracer {
    GThreadPool *worker_thread_pool;
    GAsyncQueue *result_queue;

    GMutex lock;
    GCond cond;
    TracerFlag flag;
};

typedef struct TraceParams {
    gboolean continuous;
    gchar **args;
} TraceParams;


static void tracer_set_flag(Tracer *tracer, TracerFlag flag) {
    g_mutex_lock(&tracer->lock);
    tracer->flag = flag;
    g_cond_signal(&tracer->cond);
    g_mutex_unlock(&tracer->lock);
}

static void tracer_queue_syscall(Tracer *tracer, SyscallInfo *syscall) {
    TraceResult *result = g_malloc(sizeof(TraceResult));
    *result = (TraceResult){type: TRACEE_SYSCALL, syscall: *syscall};
    g_async_queue_push(tracer->result_queue, result);
}

static void tracer_queue_exit(Tracer *tracer, int status) {
    TraceResult *result = g_malloc(sizeof(TraceResult));
    *result = (TraceResult){type: TRACEE_EXIT, exit_status: status};
    g_async_queue_push(tracer->result_queue, result);
}

static void tracer_queue_term(Tracer *tracer, int signal) {
    TraceResult *result = g_malloc(sizeof(TraceResult));
    *result = (TraceResult){type: TRACEE_TERMINATED, term_signal: signal};
    g_async_queue_push(tracer->result_queue, result);
}

static void tracer_clear_result_queue(Tracer *tracer) {
    g_async_queue_lock(tracer->result_queue);
    gpointer ptr;
    while (ptr = g_async_queue_try_pop_unlocked(tracer->result_queue)) {
        g_free(ptr);
    }
    g_async_queue_unlock(tracer->result_queue);
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

    tracer_clear_result_queue(tracer);
    pid_t child = start_tracee(params->args[0], params->args);
    if (child > 0) {
        tracer_set_flag(tracer, CONTINUE_FLAG);

        SyscallInfo syscall;
        ptrace(PTRACE_SYSCALL, child, 0, 0);
        while (TRUE) {
            int status;
            if (waitpid(child, &status, WNOHANG) == 0)
                continue;
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
                        if (!params->continuous)
                            tracer_set_flag(tracer, PAUSE_FLAG);
                        break;
                    case PTRACE_SYSCALL_INFO_SECCOMP:
                        syscall.number = sc_info.seccomp.nr;
                        for (int i = 0; i < 6; ++i)
                            syscall.args[i] = sc_info.seccomp.args[i];
                        syscall.retval = sc_info.seccomp.ret_data;
                        syscall.has_retval = TRUE;
                        tracer_queue_syscall(tracer, &syscall);
                        if (!params->continuous)
                            tracer_set_flag(tracer, PAUSE_FLAG);
                        break;
                }
            }

            g_mutex_lock(&tracer->lock);
            while (tracer->flag == PAUSE_FLAG)
                g_cond_wait(&tracer->cond, &tracer->lock);
            if (tracer->flag == KILL_FLAG) {
                kill(child, SIGKILL);
                while (TRUE) {
                    waitpid(child, &status, 0);
                    if (WIFSIGNALED(status)) {
                        tracer_queue_term(tracer, WTERMSIG(status));
                        break;
                    }
                }
                g_mutex_unlock(&tracer->lock);
                break;
            }
            g_mutex_unlock(&tracer->lock);

            ptrace(PTRACE_SYSCALL, child, 0, 0);
        }
    }

    g_strfreev(params->args);
    g_free(params);
}


Tracer *tracer_new() {
    Tracer *tracer = g_malloc(sizeof(Tracer));
    tracer->worker_thread_pool = g_thread_pool_new(worker_thread, tracer, 1, TRUE, NULL);
    tracer->result_queue = g_async_queue_new_full(g_free);
    g_mutex_init(&tracer->lock);
    g_cond_init(&tracer->cond);
    return tracer;
}

void tracer_free(Tracer *tracer) {
    tracer_kill_child_proc(tracer);
    g_thread_pool_free(tracer->worker_thread_pool, TRUE, TRUE);
    g_async_queue_unref(tracer->result_queue);
    g_cond_clear(&tracer->cond);
    g_mutex_clear(&tracer->lock);
    g_free(tracer);
}

void tracer_kill_child_proc(Tracer *tracer) {
    tracer_set_flag(tracer, KILL_FLAG);
}

TraceResult *tracer_pop_queued_result(Tracer *tracer) {
    return g_async_queue_try_pop(tracer->result_queue);
}

GQueue tracer_get_queued_results(Tracer *tracer) {
    GQueue queue;
    g_queue_init(&queue);

    g_async_queue_lock(tracer->result_queue);
    gpointer data;
    while (data = g_async_queue_try_pop_unlocked(tracer->result_queue)) {
        g_queue_push_tail(&queue, data);
    }
    g_async_queue_unlock(tracer->result_queue);

    return queue;
}

gboolean tracer_start_trace_async(Tracer *tracer, gchar **args, gboolean continuous) {
    TraceParams *params = g_malloc(sizeof(TraceParams));
    *params = (TraceParams) {continuous: continuous, args: g_strdupv(args)};
    return g_thread_pool_push(tracer->worker_thread_pool, params, NULL);
}

void tracer_trace_next(Tracer *tracer) {
    tracer_set_flag(tracer, CONTINUE_FLAG);
}