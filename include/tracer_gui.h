#pragma once

#include "tracer_app.h"

void tracer_gui_init();

void tracer_gui_new_syscall(TraceResult *trace);

void tracer_gui_on_trace_finish();