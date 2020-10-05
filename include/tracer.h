#pragma once

#include <ctype.h>
#include <glib.h>

struct Tracer;
typedef struct Tracer Tracer;

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

Tracer *tracer_new();

void tracer_free(Tracer *tracer);

gboolean tracer_start_trace_async(Tracer *tracer, gchar **args, gboolean continuous);

void tracer_kill_child_proc(Tracer *tracer);

void tracer_resume_trace(Tracer *tracer);

TraceResult *tracer_pop_queued_result(Tracer *tracer);

GQueue tracer_get_queued_results(Tracer *tracer);

gboolean tracer_is_paused(Tracer *tracer);