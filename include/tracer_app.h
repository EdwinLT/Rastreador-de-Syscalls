#pragma once

#include <ctype.h>
#include <glib.h>

typedef struct TraceResult_t {
    int64_t sysno;
    int64_t args[6];
    gboolean has_retval;
    int64_t retval;
} TraceResult;

void tracer_app_init(void);

void tracer_app_quit(void);

gboolean tracer_app_start_trace(gchar **args, gboolean continuous);

void tracer_app_kill_child_proc(void);

void tracer_app_trace_next(void);