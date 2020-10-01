#include "tracer_gui.h"
#include "tracer_app.h"

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
    GtkTreeView *chart_legend;
    GtkListStore *log_liststore;
    GtkListStore *stats_liststore;
    GtkListStore *chart_liststore;
    GtkTreeSelection *chart_selection;
    GtkWidget *chart_drawing_area;
    gulong syscall_counter;
    gdouble chart_radius;
    guint refresh_source_id;
} gui;
#define GUI_BIND_OBJECT(builder, class, obj) (gui.obj = class(gtk_builder_get_object(builder, #obj)))

static void tracer_gui_log_syscall(SyscallInfo*);
static void tracer_gui_finish_trace(void);

#define REFRESH_INTERVAL (1000 / 30)
static gboolean refresh_gui_source_func(gpointer data) {
    TraceResult *trace;
    while (trace = tracer_app_pop_queued_result()) {
        switch (trace->type) {
            case TRACEE_SYSCALL:
                tracer_gui_log_syscall(&trace->syscall);
                break;
            case TRACEE_EXIT:
                printf("Exit status: %d\n", trace->exit_status);
                tracer_gui_finish_trace();
                break;
            case TRACEE_TERMINATED:
                printf("Stop signal: %d\n", trace->term_signal);
                tracer_gui_finish_trace();
                break;
        }
        g_free(trace);
    }
    return G_SOURCE_CONTINUE;
}

static void tracer_gui_log_syscall(SyscallInfo *syscall) {
    const gchar *name = (const gchar*) syscall_name(syscall->number);
    GtkTreeIter iter;

    // Update log
    gtk_list_store_insert_with_values(gui.log_liststore, &iter, -1,
        LOG_COL_INDEX, gui.syscall_counter++,
        LOG_COL_SYSNO, (gint64) syscall->number,
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
        if (sysno == syscall->number) {
            gtk_list_store_set(gui.stats_liststore, &iter, 2, count + 1, -1);
            not_in_table = FALSE;
            break;
        }
        valid = gtk_tree_model_iter_next(tm, &iter);
    }
    if (not_in_table) {
        gtk_list_store_insert_with_values(gui.stats_liststore, NULL, -1,
            STATS_COL_SYSNO, (gint64) syscall->number,
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
        gdouble percent = (gdouble) count / gui.syscall_counter * 100.0;

        GdkRGBA rgba = hsv_to_rgb(hues[i], 0.62, 0.9);
        gtk_list_store_insert_with_values(gui.chart_liststore, NULL, -1,
            CHART_COL_SYSNO, sysno,
            CHART_COL_NAME, syscall_name(sysno),
            CHART_COL_PERCENT, percent, 
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

static void tracer_gui_finish_trace(void) {
    gtk_widget_set_sensitive(GTK_WIDGET(gui.back_button), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui.proc_control_box), FALSE);
    tracer_gui_set_pie_chart_data();
}


void on_main_window_destroy(void) {
    tracer_app_quit();
    g_source_remove(gui.refresh_source_id);
    gtk_main_quit();
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
}

void on_next_syscall_button_clicked(GtkButton *btn, gpointer data) {
    tracer_app_trace_next();
    gtk_widget_set_sensitive(GTK_WIDGET(gui.next_syscall_button), FALSE);
}

void on_back_button_clicked(GtkButton *btn, gpointer data) {
    gtk_stack_set_visible_child_name(gui.main_stack, "start_page");
    gtk_widget_grab_default(GTK_WIDGET(gui.start_button));
}


static void pie_chart_slice(cairo_t *cr, double xc, double yc, 
                            double r, double angle1, double angle2) {
    cairo_move_to(cr, xc, yc);
    cairo_arc(cr, xc, yc, r, angle1, angle2);
    cairo_line_to(cr, xc, yc);
}

#define TAU (2 * G_PI)

static void draw_pie_chart(cairo_t *cr, double radius, double radius_selected) {
    GtkTreeModel *chart_model = GTK_TREE_MODEL(gui.chart_liststore);
    GtkTreeIter iter;

    gboolean has_selection = gtk_tree_selection_get_selected(gui.chart_selection, NULL, &iter);
    gint64 selected;
    if (has_selection) {
        gtk_tree_model_get(chart_model, &iter, CHART_COL_SYSNO, &selected, -1);
    }

    double angle = 0.0;
    double slice_radius = radius;
    gboolean valid = gtk_tree_model_get_iter_first(chart_model, &iter);
    while (valid) {
        gint64 sysno;
        gdouble percent;
        gdouble r, g, b;
        gtk_tree_model_get(chart_model, &iter,
            CHART_COL_SYSNO, &sysno, CHART_COL_PERCENT, &percent,
            CHART_COL_R, &r, CHART_COL_G, &g, CHART_COL_B, &b,
            -1
        );

        if (has_selection)
            slice_radius = (selected == sysno)? radius_selected : radius;

        double end_angle = angle + (percent/100.0)*(TAU);
        pie_chart_slice(cr, 0.0, 0.0, slice_radius, angle, end_angle);
        cairo_set_source_rgb(cr, r, g, b);
        cairo_fill_preserve(cr);
        cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.5);
        cairo_stroke(cr);

        angle = end_angle;
        valid = gtk_tree_model_iter_next(chart_model, &iter);
    }
}

gboolean on_chart_drawing_area_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    double half_width  = (double)gtk_widget_get_allocated_width(widget)  * 0.5;
    double half_height = (double)gtk_widget_get_allocated_height(widget) * 0.5;
    gui.chart_radius = fmin(half_width, half_height) * 0.9;
    cairo_translate(cr, half_width, half_height);
    cairo_set_line_width(cr, 1.0);
    draw_pie_chart(cr, gui.chart_radius, gui.chart_radius * 1.1);
    return FALSE;
}

void on_chart_selection_changed(GtkTreeSelection *select, gpointer data) {
    gtk_widget_queue_draw(gui.chart_drawing_area);
}

static gboolean on_chart_clicked(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    if (event->button == GDK_BUTTON_PRIMARY) {
        gdouble x = event->x - (gdouble)gtk_widget_get_allocated_width(widget)*0.5;
        gdouble y = event->y - (gdouble)gtk_widget_get_allocated_height(widget)*0.5;

        if (sqrt(x*x + y*y) <= gui.chart_radius) {
            gdouble click_angle = fmod(atan2(y, x)+TAU, TAU);
            GtkTreeModel *tm = GTK_TREE_MODEL(gui.chart_liststore);
            GtkTreeIter iter;
            gdouble iter_angle = 0.0;
            gboolean valid = gtk_tree_model_get_iter_first(tm, &iter);
            while (valid) {
                gdouble percent;
                gtk_tree_model_get(tm, &iter, CHART_COL_PERCENT, &percent, -1);

                gdouble end_angle = iter_angle + (percent/100.0)*(TAU);
                if (iter_angle <= click_angle && click_angle <= end_angle) {
                    GtkTreePath *path = gtk_tree_model_get_path(tm, &iter);
                    gtk_tree_selection_select_iter(gui.chart_selection, &iter);
                    gtk_tree_view_scroll_to_cell(gui.chart_legend, path, NULL, FALSE, 0.0, 0.0);
                    gtk_tree_path_free(path);
                    break;
                }
                iter_angle = end_angle;
                valid = gtk_tree_model_iter_next(tm, &iter);
            }
        }
    }
    return TRUE;
}


static void tracer_gui_init(void) {
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
    GUI_BIND_OBJECT(builder, GTK_TREE_VIEW, chart_legend);
    GUI_BIND_OBJECT(builder, GTK_LIST_STORE, log_liststore);
    GUI_BIND_OBJECT(builder, GTK_LIST_STORE, stats_liststore);
    GUI_BIND_OBJECT(builder, GTK_LIST_STORE, chart_liststore);
    GUI_BIND_OBJECT(builder, GTK_TREE_SELECTION, chart_selection);
    GUI_BIND_OBJECT(builder, GTK_WIDGET, chart_drawing_area);
    gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(gui.chart_liststore),
                                         CHART_COL_PERCENT, GTK_SORT_DESCENDING);
    gtk_widget_add_events(gui.chart_drawing_area, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(gui.chart_drawing_area, "button-press-event", 
                     G_CALLBACK(on_chart_clicked), NULL);

    g_object_unref(builder);
    gtk_window_present(gui.main_window);
    gui.refresh_source_id = g_timeout_add(REFRESH_INTERVAL, refresh_gui_source_func, NULL);
}

void tracer_gui_launch(int argc, char **argv) {
    gtk_init(NULL, NULL);
    tracer_gui_init();
    gtk_main();
}