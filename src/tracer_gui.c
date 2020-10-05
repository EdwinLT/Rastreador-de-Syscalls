#include "tracer_gui.h"
#include "tracer.h"

#include "syscall_names.h"
#include "colors.h"
#include "columns.h"

#include <math.h>
#include <gtk/gtk.h>

typedef struct TracerGui {
    Tracer *tracer;

    GtkWindow *main_window;
    GtkBox *input_box;
    GtkEntry *command_entry;
    GtkComboBox *mode_combobox;
    GtkButton *start_button;
    GtkButton *next_syscall_button;
    GtkButton *stop_button;
    GtkNotebook *stats_notebook;
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
} TracerGui;


static void tracer_gui_log_syscall(TracerGui *gui, SyscallInfo *syscall) {
    const gchar *name = (const gchar*) syscall_name(syscall->number);
    GtkTreeIter iter;

    // Update log
    gtk_list_store_insert_with_values(gui->log_liststore, &iter, -1,
        LOG_COL_INDEX, gui->syscall_counter++,
        LOG_COL_SYSNO, (gint64) syscall->number,
        LOG_COL_NAME, name,
        -1
    );
    GtkTreePath *new_row_path = gtk_tree_model_get_path(GTK_TREE_MODEL(gui->log_liststore), &iter);
    gtk_tree_view_scroll_to_cell(gui->log_treeview, new_row_path, NULL, FALSE, 0.0, 0.0);
    gtk_tree_path_free(new_row_path);

    // Update stats table
    gboolean found_in_table = FALSE;
    GtkTreeModel *tm = GTK_TREE_MODEL(gui->stats_liststore);
    gboolean valid = gtk_tree_model_get_iter_first(tm, &iter);
    while (valid) {
        glong sysno; guint count;
        gtk_tree_model_get(tm, &iter, STATS_COL_SYSNO, &sysno, STATS_COL_COUNT, &count, -1);
        if (sysno == syscall->number) {
            gtk_list_store_set(gui->stats_liststore, &iter, 2, count + 1, -1);
            found_in_table = TRUE;
            break;
        }
        valid = gtk_tree_model_iter_next(tm, &iter);
    }
    if (!found_in_table) {
        gtk_list_store_insert_with_values(gui->stats_liststore, NULL, -1,
            STATS_COL_SYSNO, (gint64) syscall->number,
            STATS_COL_NAME,  name,
            STATS_COL_COUNT, (guint) 1U,
            -1
        );
    }
}

static void tracer_gui_set_pie_chart_data(TracerGui *gui) {
    gtk_list_store_clear(gui->chart_liststore);
    GtkTreeModel *stats_model = GTK_TREE_MODEL(gui->stats_liststore);
    guint n = gtk_tree_model_iter_n_children(stats_model, NULL);
    gdouble hues[n];
    create_n_hues(n, TRUE, hues);

    guint i = 0;
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(stats_model, &iter);
    while (valid) {
        gint64 sysno; guint count;
        gtk_tree_model_get(stats_model, &iter, STATS_COL_SYSNO, &sysno, STATS_COL_COUNT, &count, -1);
        gdouble proportion = (gdouble) count / gui->syscall_counter;

        GdkRGBA rgba = hsv_to_rgb(hues[i], 0.62, 0.9);
        gtk_list_store_insert_with_values(gui->chart_liststore, NULL, -1,
            CHART_COL_SYSNO, sysno,
            CHART_COL_NAME, syscall_name(sysno),
            CHART_COL_PROPORTION, proportion, 
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

    gtk_widget_queue_draw(gui->chart_drawing_area);
}

static void tracer_gui_finish_trace(TracerGui *gui) {
    gtk_widget_set_sensitive(GTK_WIDGET(gui->next_syscall_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->stop_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->input_box), TRUE);
    gtk_widget_grab_default(GTK_WIDGET(gui->start_button));
    tracer_gui_set_pie_chart_data(gui);
}

static void tracer_gui_start_trace(TracerGui *gui, gchar **argv, gboolean continuous) {
    if (tracer_start_trace_async(gui->tracer, argv, continuous)) {
        gui->syscall_counter = 0UL;
        gtk_list_store_clear(gui->log_liststore);
        gtk_list_store_clear(gui->stats_liststore);
        gtk_list_store_clear(gui->chart_liststore);
        gtk_widget_set_sensitive(GTK_WIDGET(gui->next_syscall_button), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(gui->input_box), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(gui->stop_button), TRUE);
        gtk_notebook_set_current_page(gui->stats_notebook, 0);
        gtk_widget_grab_default(GTK_WIDGET(gui->next_syscall_button));
    }
}


void on_main_window_destroy(GtkWidget *widget, gpointer data) {
    TracerGui *gui = data;
    tracer_kill_child_proc(gui->tracer);
    g_source_remove(gui->refresh_source_id);
    gtk_main_quit();
}

void on_start_button_clicked(GtkButton *btn, gpointer data) {
    TracerGui *gui = data;
    const gchar *cmd = gtk_entry_get_text(gui->command_entry);
    gint argc; gchar **argv;

    if (g_shell_parse_argv(cmd, &argc, &argv, NULL)) {
        const char *mode_id = gtk_combo_box_get_active_id(gui->mode_combobox);
        gboolean continuous = (g_strcmp0(mode_id, "CONTINUOUS") == 0);
        tracer_gui_start_trace(gui, argv, continuous);
        g_strfreev(argv);
    }
}

void on_stop_button_clicked(GtkButton *btn, gpointer data) {
    TracerGui *gui = data;
    tracer_kill_child_proc(gui->tracer);
}

void on_next_syscall_button_clicked(GtkButton *btn, gpointer data) {
    TracerGui *gui = data;
    tracer_resume_trace(gui->tracer);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->next_syscall_button), FALSE);
}

void on_close_button_clicked(GtkButton *btn, gpointer data) {
    TracerGui *gui = data;
    gtk_widget_destroy(GTK_WIDGET(gui->main_window));
}


#define TAU (2 * G_PI)

static void tracer_gui_draw_pie_chart(TracerGui *gui, cairo_t *cr, double radius, double radius_selected) {
    GtkTreeModel *model = GTK_TREE_MODEL(gui->chart_liststore);

    GtkTreeIter iter;
    double start_angle = -G_PI;
    gboolean valid = gtk_tree_model_get_iter_first(model, &iter);
    while (valid) {
        gint64 sysno;
        gdouble proportion;
        gdouble r, g, b;
        gtk_tree_model_get(model, &iter,
            CHART_COL_SYSNO, &sysno, CHART_COL_PROPORTION, &proportion,
            CHART_COL_R, &r, CHART_COL_G, &g, CHART_COL_B, &b,
            -1
        );

        double slice_radius;
        if (gtk_tree_selection_iter_is_selected(gui->chart_selection, &iter)) {
            cairo_set_line_width(cr, 2.0);
            slice_radius = radius_selected;
        } else {
            cairo_set_line_width(cr, 1.0);
            slice_radius = radius;
        }

        double end_angle = start_angle + proportion * TAU;
        cairo_move_to(cr, 0.0, 0.0);
        cairo_arc(cr, 0.0, 0.0, slice_radius, start_angle, end_angle);
        cairo_line_to(cr, 0.0, 0.0);

        cairo_set_source_rgb(cr, r, g, b);
        cairo_fill_preserve(cr);
        cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.5);
        cairo_stroke(cr);

        start_angle = end_angle;
        valid = gtk_tree_model_iter_next(model, &iter);
    }
    gui->chart_radius = radius;
}

gboolean on_chart_drawing_area_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    double half_width  = (double)gtk_widget_get_allocated_width(widget)  * 0.5;
    double half_height = (double)gtk_widget_get_allocated_height(widget) * 0.5;
    double r = fmin(half_width, half_height);
    cairo_translate(cr, half_width, half_height);
    cairo_set_line_width(cr, 1.0);
    tracer_gui_draw_pie_chart(data, cr, r * 0.85, r * 0.95);
    return FALSE;
}

void on_chart_selection_changed(GtkTreeSelection *select, gpointer data) {
    TracerGui *gui = data;
    gtk_widget_queue_draw(gui->chart_drawing_area);
}

static gboolean on_chart_clicked(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    if (event->button == GDK_BUTTON_PRIMARY) {
        gdouble x = event->x - (gdouble)gtk_widget_get_allocated_width(widget)*0.5;
        gdouble y = event->y - (gdouble)gtk_widget_get_allocated_height(widget)*0.5;
        TracerGui *gui = data;

        if (sqrt(x*x + y*y) <= gui->chart_radius) {
            gdouble click_angle = atan2(y, x);
            GtkTreeModel *tm = GTK_TREE_MODEL(gui->chart_liststore);
            GtkTreeIter iter;
            gdouble iter_angle = -G_PI;
            gboolean valid = gtk_tree_model_get_iter_first(tm, &iter);
            while (valid) {
                gdouble proportion;
                gtk_tree_model_get(tm, &iter, CHART_COL_PROPORTION, &proportion, -1);

                gdouble end_angle = iter_angle + proportion * TAU;
                if (iter_angle <= click_angle && click_angle <= end_angle) {
                    GtkTreePath *path = gtk_tree_model_get_path(tm, &iter);
                    gtk_tree_selection_select_iter(gui->chart_selection, &iter);
                    gtk_tree_view_scroll_to_cell(gui->chart_legend, path, NULL, FALSE, 0.0, 0.0);
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


#define REFRESH_INTERVAL (1000 / 30)
static gboolean refresh_gui_source_func(gpointer data) {
    TracerGui *gui = data;
    GQueue queue = tracer_get_queued_results(gui->tracer);
    if (queue.length > 0) {
        TraceResult *trace;
        while (trace = g_queue_pop_head(&queue)) {
            switch (trace->type) {
                case TRACEE_SYSCALL:
                    tracer_gui_log_syscall(gui, &trace->syscall);
                    gtk_widget_set_sensitive(GTK_WIDGET(gui->next_syscall_button), 
                                             tracer_is_paused(gui->tracer));
                    break;
                case TRACEE_EXIT:
                case TRACEE_TERMINATED:
                    tracer_gui_finish_trace(gui);
                    break;
            }
            g_free(trace);
        }
    }
    return G_SOURCE_CONTINUE;
}

#define GUI_BIND_OBJECT(gui, builder, class, obj) (gui->obj = class(gtk_builder_get_object(builder, #obj)))
static void tracer_gui_init_widgets(TracerGui *gui) {
    GtkBuilder *builder = gtk_builder_new_from_resource("/com/github/edwinlt/Rastreador/tracer.glade");
    gtk_builder_connect_signals(builder, gui);

    GUI_BIND_OBJECT(gui, builder, GTK_WINDOW, main_window);
    GUI_BIND_OBJECT(gui, builder, GTK_BOX, input_box);
    GUI_BIND_OBJECT(gui, builder, GTK_ENTRY, command_entry);
    GUI_BIND_OBJECT(gui, builder, GTK_COMBO_BOX, mode_combobox);
    GUI_BIND_OBJECT(gui, builder, GTK_BUTTON, start_button);
    GUI_BIND_OBJECT(gui, builder, GTK_BUTTON, next_syscall_button);
    GUI_BIND_OBJECT(gui, builder, GTK_BUTTON, stop_button);
    GUI_BIND_OBJECT(gui, builder, GTK_NOTEBOOK, stats_notebook);
    GUI_BIND_OBJECT(gui, builder, GTK_TREE_VIEW, log_treeview);
    GUI_BIND_OBJECT(gui, builder, GTK_TREE_VIEW, stats_treeview);
    GUI_BIND_OBJECT(gui, builder, GTK_TREE_VIEW, chart_legend);
    GUI_BIND_OBJECT(gui, builder, GTK_LIST_STORE, log_liststore);
    GUI_BIND_OBJECT(gui, builder, GTK_LIST_STORE, stats_liststore);
    GUI_BIND_OBJECT(gui, builder, GTK_LIST_STORE, chart_liststore);
    GUI_BIND_OBJECT(gui, builder, GTK_TREE_SELECTION, chart_selection);
    GUI_BIND_OBJECT(gui, builder, GTK_WIDGET, chart_drawing_area);
    gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(gui->chart_liststore),
                                         CHART_COL_PROPORTION, GTK_SORT_DESCENDING);
    gtk_widget_add_events(gui->chart_drawing_area, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(gui->chart_drawing_area, "button-press-event", 
                     G_CALLBACK(on_chart_clicked), gui);

    g_object_unref(builder);
    gtk_window_present(gui->main_window);
    gui->refresh_source_id = g_timeout_add(REFRESH_INTERVAL, refresh_gui_source_func, gui);
}

void tracer_gui_main(int argc, char **argv) {
    gtk_init(NULL, NULL);

    TracerGui *gui = g_malloc(sizeof(TracerGui));
    gui->tracer = tracer_new();
    tracer_gui_init_widgets(gui);

    if (argc > 0) {
        tracer_gui_start_trace(gui, argv, TRUE);
    }

    gtk_main();

    tracer_free(gui->tracer);
    g_free(gui);
}