#pragma once

#include <ctype.h>
#include <glib.h>

typedef enum {
    TRACEE_SYSCALL,
    TRACEE_EXIT,
    TRACEE_TERMINATED
} TraceResultType;

typedef struct SyscallInfo {
    int64_t number;
    int64_t args[6];
    gboolean has_retval;
    int64_t retval;
} SyscallInfo;

typedef struct TraceResult {
    TraceResultType type;
    union {
        SyscallInfo syscall;
        int exit_status;
        int term_signal;
    };
} TraceResult;

void tracer_app_init(void);

void tracer_app_quit(void);

gboolean tracer_app_start_trace(gchar **args, gboolean continuous);

void tracer_app_kill_child_proc(void);

void tracer_app_trace_next(void);

TraceResult *tracer_app_pop_queued_result(void);