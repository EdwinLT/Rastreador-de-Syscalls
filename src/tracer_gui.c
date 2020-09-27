#include "tracer_gui.h"
#include "tracer_app.h"

#include <gtk/gtk.h>

static struct {
    GtkWindow *main_window;
    GtkStack *main_stack;
    GtkStack *process_stack;
    GtkEntry *command_entry;
    GtkComboBox *mode_combobox;
    GtkButton *start_button;
    GtkButton *back_button;
    GtkButton *next_syscall_button;
    GtkButtonBox *proc_control_box;
    GtkTreeView *log_treeview;
    GtkTreeView *stats_treeview;
    GtkListStore *log_liststore;
    GtkListStore *stats_liststore;
    gulong log_rows_counter;
} gui;
#define GUI_BIND_OBJECT(builder, class, obj) (gui.obj = class(gtk_builder_get_object(builder, #obj)))

void tracer_gui_init() {
    GtkBuilder *builder = gtk_builder_new_from_resource("/com/github/edwinlt/Rastreador/window.glade");
    gtk_builder_connect_signals(builder, NULL);

    GUI_BIND_OBJECT(builder, GTK_WINDOW, main_window);
    GUI_BIND_OBJECT(builder, GTK_STACK, main_stack);
    GUI_BIND_OBJECT(builder, GTK_STACK, process_stack);
    GUI_BIND_OBJECT(builder, GTK_ENTRY, command_entry);
    GUI_BIND_OBJECT(builder, GTK_COMBO_BOX, mode_combobox);
    GUI_BIND_OBJECT(builder, GTK_BUTTON, start_button);
    GUI_BIND_OBJECT(builder, GTK_BUTTON, back_button);
    GUI_BIND_OBJECT(builder, GTK_BUTTON, next_syscall_button);
    GUI_BIND_OBJECT(builder, GTK_BUTTON_BOX, proc_control_box);
    GUI_BIND_OBJECT(builder, GTK_TREE_VIEW, log_treeview);
    GUI_BIND_OBJECT(builder, GTK_TREE_VIEW, stats_treeview);
    GUI_BIND_OBJECT(builder, GTK_LIST_STORE, log_liststore);
    GUI_BIND_OBJECT(builder, GTK_LIST_STORE, stats_liststore);

    g_object_unref(builder);
    gtk_window_present(gui.main_window);
}


void on_main_window_destroy() {
    gtk_main_quit();
}

void on_start_button_clicked(GtkButton *btn, gpointer data) {

}

void on_stop_button_clicked(GtkButton *btn, gpointer data) {

}

void on_next_syscall_button_clicked(GtkButton *btn, gpointer data) {

}

void on_back_button_clicked(GtkButton *btn, gpointer data) {

}