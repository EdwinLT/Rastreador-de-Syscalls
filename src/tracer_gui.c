#include "tracer_gui.h"
#include "syscall_names.h"

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

void tracer_gui_report_syscall(TraceResult *trace) {
    GtkTreeIter iter;
    gtk_list_store_insert_with_values(gui.log_liststore, &iter, -1,
        0, gui.log_rows_counter++,
        1, trace->sysno,
        2, syscall_name(trace->sysno),
        -1
    );
    GtkTreePath *new_row_path = gtk_tree_model_get_path(GTK_TREE_MODEL(gui.log_liststore), &iter);
    gtk_tree_view_scroll_to_cell(gui.log_treeview, new_row_path, NULL, FALSE, 0.0, 0.0);
    gtk_tree_path_free(new_row_path);
}


void on_main_window_destroy() {
    tracer_app_quit();
}

void on_start_button_clicked(GtkButton *btn, gpointer data) {
    const gchar *cmd = gtk_entry_get_text(gui.command_entry);
    gint argc; gchar **argv;

    if (g_shell_parse_argv(cmd, &argc, &argv, NULL)) {
        const char *mode_id = gtk_combo_box_get_active_id(gui.mode_combobox);
        gboolean continuous = (g_strcmp0(mode_id, "CONTINUOUS") == 0);

        if (tracer_app_start_trace(argv, continuous)) {
            gui.log_rows_counter = 0UL;
            gtk_list_store_clear(gui.log_liststore);
            gtk_list_store_clear(gui.stats_liststore);
            gtk_widget_set_visible(GTK_WIDGET(gui.next_syscall_button), !continuous);
            gtk_widget_set_sensitive(GTK_WIDGET(gui.back_button), FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(gui.proc_control_box), TRUE);
            gtk_stack_set_visible_child_name(gui.process_stack, "tables_view");
            gtk_stack_set_visible_child_name(gui.main_stack, "process_view");
        }

        g_strfreev(argv);
    }
}

void on_stop_button_clicked(GtkButton *btn, gpointer data) {

}

void on_next_syscall_button_clicked(GtkButton *btn, gpointer data) {

}

void on_back_button_clicked(GtkButton *btn, gpointer data) {

}