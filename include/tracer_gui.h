#pragma once

#include "tracer_app.h"

void tracer_gui_launch(int argc, char **argv);

void tracer_gui_log_syscall(TraceResult *trace);

void tracer_gui_on_trace_finish(void);