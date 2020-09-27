#include "tracer_app.h"
#include "tracer_gui.h"

#include <gtk/gtk.h>

void tracer_app_run(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    tracer_gui_init();
    gtk_main();
}