#include "tracer_gui.h"
#include "syscall_names.h"
#include "colors.h"
#include "columns.h"

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
    GtkListStore *chart_liststore;
    GtkWidget *chart_drawing_area;
    gulong syscall_counter;
} gui;
#define GUI_BIND_OBJECT(builder, class, obj) (gui.obj = class(gtk_builder_get_object(builder, #obj)))

void tracer_gui_init(void) {
    GtkBuilder *builder = gtk_builder_new_from_resource("/com/github/edwinlt/Rastreador/window.glade");
    gtk_builder_connect_signals(builder, NULL);

    GUI_BIND_OBJECT(builder, GTK_WINDOW,     main_window);
    GUI_BIND_OBJECT(builder, GTK_STACK,      main_stack);
    GUI_BIND_OBJECT(builder, GTK_STACK,      process_stack);
    GUI_BIND_OBJECT(builder, GTK_ENTRY,      command_entry);
    GUI_BIND_OBJECT(builder, GTK_COMBO_BOX,  mode_combobox);
    GUI_BIND_OBJECT(builder, GTK_BUTTON,     start_button);
    GUI_BIND_OBJECT(builder, GTK_BUTTON,     back_button);
    GUI_BIND_OBJECT(builder, GTK_BUTTON,     next_syscall_button);
    GUI_BIND_OBJECT(builder, GTK_BUTTON_BOX, proc_control_box);
    GUI_BIND_OBJECT(builder, GTK_TREE_VIEW,  log_treeview);
    GUI_BIND_OBJECT(builder, GTK_TREE_VIEW,  stats_treeview);
    GUI_BIND_OBJECT(builder, GTK_LIST_STORE, log_liststore);
    GUI_BIND_OBJECT(builder, GTK_LIST_STORE, stats_liststore);
    GUI_BIND_OBJECT(builder, GTK_LIST_STORE, chart_liststore);
    GUI_BIND_OBJECT(builder, GTK_WIDGET,     chart_drawing_area);

    g_object_unref(builder);
    gtk_window_present(gui.main_window);
}

void tracer_gui_log_syscall(TraceResult *trace) {
    const gchar *name = (const gchar*) syscall_name(trace->sysno);
    GtkTreeIter iter;

    // Update log
    gtk_list_store_insert_with_values(gui.log_liststore, &iter, -1,
        LOG_COL_INDEX, gui.syscall_counter++,
        LOG_COL_SYSNO, (gint64) trace->sysno,
        LOG_COL_NAME, name,
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
        gtk_tree_model_get(tm, &iter, STATS_COL_SYSNO, &sysno, STATS_COL_COUNT, &count, -1);
        if (sysno == trace->sysno) {
            gtk_list_store_set(gui.stats_liststore, &iter, 2, count + 1, -1);
            not_in_table = FALSE;
            break;
        }
        valid = gtk_tree_model_iter_next(tm, &iter);
    }
    if (not_in_table) {
        gtk_list_store_insert_with_values(gui.stats_liststore, NULL, -1,
            STATS_COL_SYSNO, (gint64) trace->sysno,
            STATS_COL_NAME,  name,
            STATS_COL_COUNT, (guint) 1U,
            -1
        );
    }

    gtk_widget_set_sensitive(GTK_WIDGET(gui.next_syscall_button), TRUE);
}

static void tracer_gui_set_pie_chart_data(void) {
    GtkTreeModel *stats_model = GTK_TREE_MODEL(gui.stats_liststore);
    guint n = gtk_tree_model_iter_n_children(stats_model, NULL);
    double *hues = create_n_hues(n, TRUE);

    guint i = 0;
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(stats_model, &iter);
    while (valid) {
        gint64 sysno; guint count;
        gtk_tree_model_get(stats_model, &iter, STATS_COL_SYSNO, &sysno, STATS_COL_COUNT, &count, -1);
        gdouble proportion = (gdouble) count / gui.syscall_counter;

        GdkRGBA rgba = hsv_to_rgb(hues[i], 0.62, 0.9);
        gtk_list_store_insert_with_values(gui.chart_liststore, NULL, -1,
            CHART_COL_SYSNO, sysno,
            CHART_COL_NAME, syscall_name(sysno),
            CHART_COL_PROP, proportion, 
            CHART_COL_RGBA, &rgba,
            // Because of a bug in gtk, we can't properly retrieve the GdkRGBA
            // To get around this, we store the r, g, and b separately as well
            CHART_COL_R, rgba.red,
            CHART_COL_G, rgba.green,
            CHART_COL_B, rgba.blue,
            -1
        );

        i++;
        valid = gtk_tree_model_iter_next(stats_model, &iter);
    }

    gtk_widget_queue_draw(gui.chart_drawing_area);
    free(hues);
}

void tracer_gui_on_trace_finish(void) {
    gtk_widget_set_sensitive(GTK_WIDGET(gui.back_button), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui.proc_control_box), FALSE);
    tracer_gui_set_pie_chart_data();
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
            gui.syscall_counter = 0UL;
            gtk_list_store_clear(gui.log_liststore);
            gtk_list_store_clear(gui.stats_liststore);
            gtk_list_store_clear(gui.chart_liststore);
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

static void pie_chart_slice(cairo_t *cr, double xc, double yc, 
                            double r, double angle1, double angle2) {
    cairo_move_to(cr, xc, yc);
    cairo_arc(cr, xc, yc, r, angle1, angle2);
    cairo_line_to(cr, xc, yc);
}

static void draw_pie_chart(cairo_t *cr, double radius) {
    double angle = 0.0;
    GtkTreeModel *chart_model = GTK_TREE_MODEL(gui.chart_liststore);
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(chart_model, &iter);
    while (valid) {
        gdouble proportion;
        gdouble r, g, b;
        gtk_tree_model_get(chart_model, &iter,
            CHART_COL_PROP, &proportion, 
            CHART_COL_R, &r, CHART_COL_G, &g, CHART_COL_B, &b,
            -1
        );

        double end_angle = angle + proportion * TAU;
        pie_chart_slice(cr, 0.0, 0.0, radius, angle, end_angle);
        cairo_set_source_rgb(cr, r, g, b);
        cairo_fill_preserve(cr);
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_stroke(cr);

        angle = end_angle;
        valid = gtk_tree_model_iter_next(chart_model, &iter);
    }
}

gboolean on_chart_drawing_area_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    double half_width  = (double)gtk_widget_get_allocated_width(widget)  * 0.5;
    double half_height = (double)gtk_widget_get_allocated_height(widget) * 0.5;
    double radius = fmin(half_width, half_height) * 0.9;
    cairo_translate(cr, half_width, half_height);
    cairo_set_line_width(cr, 1.0);
    draw_pie_chart(cr, radius);
    return FALSE;
}