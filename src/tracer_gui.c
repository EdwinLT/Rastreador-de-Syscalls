#include "tracer_gui.h"
#include "syscall_names.h"
#include "colors.h"

#include <math.h>
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

void tracer_gui_init(void) {
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

void tracer_gui_new_syscall(TraceResult *trace) {
    const gchar *name = (const gchar*) syscall_name(trace->sysno);
    GtkTreeIter iter;

    // Update log
    gtk_list_store_insert_with_values(gui.log_liststore, &iter, -1,
        0, gui.log_rows_counter++,
        1, trace->sysno,
        2, name,
        -1
    );
    GtkTreePath *new_row_path = gtk_tree_model_get_path(GTK_TREE_MODEL(gui.log_liststore), &iter);
    gtk_tree_view_scroll_to_cell(gui.log_treeview, new_row_path, NULL, FALSE, 0.0, 0.0);
    gtk_tree_path_free(new_row_path);

    // Update stats table
    gboolean not_in_table = TRUE;
    GtkTreeModel *tm = GTK_TREE_MODEL(gui.stats_liststore);
    gboolean valid = gtk_tree_model_get_iter_first(tm, &iter);
    while (valid) {
        glong sysno; guint count;
        gtk_tree_model_get(tm, &iter, 0, &sysno, 2, &count, -1);
        if (sysno == trace->sysno) {
            gtk_list_store_set(gui.stats_liststore, &iter, 2, count + 1, -1);
            not_in_table = FALSE;
            break;
        }
        valid = gtk_tree_model_iter_next(tm, &iter);
    }
    if (not_in_table) {
        gtk_list_store_insert_with_values(gui.stats_liststore, NULL, -1,
            0, (glong) trace->sysno,
            1, name,
            2, (guint) 1U,
            -1
        );
    }

    gtk_widget_set_sensitive(GTK_WIDGET(gui.next_syscall_button), TRUE);
}

void tracer_gui_on_trace_finish(void) {
    gtk_widget_set_sensitive(GTK_WIDGET(gui.back_button), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui.proc_control_box), FALSE);
}


void on_main_window_destroy(void) {
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
            gtk_widget_grab_default(GTK_WIDGET(gui.next_syscall_button));
        }

        g_strfreev(argv);
    }
}

void on_stop_button_clicked(GtkButton *btn, gpointer data) {
    tracer_app_kill_child_proc();
    tracer_gui_on_trace_finish();
}

void on_next_syscall_button_clicked(GtkButton *btn, gpointer data) {
    tracer_app_trace_next();
    gtk_widget_set_sensitive(GTK_WIDGET(gui.next_syscall_button), FALSE);
}

void on_back_button_clicked(GtkButton *btn, gpointer data) {
    gtk_stack_set_visible_child_name(gui.main_stack, "start_page");
    gtk_widget_grab_default(GTK_WIDGET(gui.start_button));
}


#define TAU (2 * M_PI)

static void draw_pie_chart_slice(cairo_t *cr, double radius, double angle1, double angle2) {
    cairo_save(cr);
    cairo_move_to(cr, 0.0, 0.0);
    cairo_line_to(cr, radius*cos(angle1), radius*sin(angle1));
    cairo_arc(cr, 0.0, 0.0, radius, angle1, angle2);
    cairo_line_to(cr, 0.0, 0.0);
    cairo_fill(cr);
    cairo_arc(cr, 0.0, 0.0, radius, angle1, angle2);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_stroke(cr);
    cairo_restore(cr);
}

static void draw_pie_chart(cairo_t *cr, double radius) {
    int n = 255;
    double *hues = create_n_hues(n, TRUE);
    double delta = TAU / n;
    cairo_set_line_width(cr, 1.0);
    for (int i = 0; i < n; i++) {
        double a1 = delta * i;
        double a2 = delta * (i+1);
        GdkRGBA color = hsv_to_rgb(hues[i], 1.0, 1.0);
        cairo_set_source_rgb(cr, color.red, color.green, color.blue);
        draw_pie_chart_slice(cr, radius, a1, a2);
    }
    free(hues);
}

gboolean on_chart_drawing_area_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    double half_width  = (double)gtk_widget_get_allocated_width(widget)  * 0.5;
    double half_height = (double)gtk_widget_get_allocated_height(widget) * 0.5;
    double radius = fmin(half_width, half_height) * 0.9;
    cairo_translate(cr, half_width, half_height);
    draw_pie_chart(cr, radius);
    return FALSE;
}