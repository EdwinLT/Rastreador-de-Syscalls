#include "tracer_gui.h"
#include "tracer_app.h"

#include <gtk/gtk.h>

static struct Gui {
    GtkWindow *main_window;
} gui;
#define GUI_BIND_OBJECT(builder, class, obj) (gui.obj = class(gtk_builder_get_object(builder, #obj)))

void tracer_gui_init() {
    GtkBuilder *builder = gtk_builder_new_from_resource("/com/github/edwinlt/Rastreador/window.glade");
    gtk_builder_connect_signals(builder, NULL);

    GUI_BIND_OBJECT(builder, GTK_WINDOW, main_window);

    g_object_unref(builder);
    gtk_window_present(gui.main_window);
}


void on_main_window_destroy() {
    gtk_main_quit();
}