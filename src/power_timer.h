#ifndef TIMED_SHUTDOWN_H
#define TIMED_SHUTDOWN_H

#include <gtk/gtk.h>
#include <adwaita.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct {
    GtkWidget *window;
    GtkWidget *hour_spin;
    GtkWidget *minute_spin;
    GtkWidget *countdown_label;
    GtkWidget *start_button;
    GtkWidget *cancel_button;
    GtkWidget *power_action_dropdown;
    guint timer_id;
    int remaining_seconds;
    gboolean is_counting;
    gboolean sound_played;
} AppData;

char* get_sound_file_path(void);
void play_sound_notification(void);

void execute_power_action(const char *action_type, const char *command);
void show_power_action_confirmation(GtkWidget *parent_window, const char *action_name, const char *command);

gboolean update_countdown(gpointer user_data);

GtkWidget* create_ui(AppData *app_data, AdwApplication *app);
void setup_keyboard_shortcuts(AppData *app_data);
void on_start_clicked(GtkWidget *widget, gpointer user_data);
void on_cancel_clicked(GtkWidget *widget, gpointer user_data);
void on_window_destroy(GtkWidget *widget, gpointer user_data);

#endif