#include "power_timer.h"

gboolean update_countdown(gpointer user_data) {
    AppData *app = (AppData *)user_data;
    
    if (app->remaining_seconds <= 0) {
        guint selected = gtk_drop_down_get_selected(GTK_DROP_DOWN(app->power_action_dropdown));
        switch (selected) {
            case 0:
                show_power_action_confirmation(app->window, "shutdown", "systemctl poweroff");
                break;
            case 1:
                show_power_action_confirmation(app->window, "restart", "systemctl reboot");
                break;
            case 2:
                show_power_action_confirmation(app->window, "log off", "loginctl terminate-user $USER");
                break;
            default:
                show_power_action_confirmation(app->window, "shutdown", "systemctl poweroff");
                break;
        }
        return G_SOURCE_REMOVE;
    }
    
    if (app->remaining_seconds <= 60 && !app->sound_played) {
        play_sound_notification();
        app->sound_played = TRUE;
    }
    
    int hours = app->remaining_seconds / 3600;
    int minutes = (app->remaining_seconds % 3600) / 60;
    int seconds = app->remaining_seconds % 60;
    
    char time_str[64];
    snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d", hours, minutes, seconds);
    
    gtk_label_set_text(GTK_LABEL(app->countdown_label), time_str);
    app->remaining_seconds--;
    
    return G_SOURCE_CONTINUE;
}

void on_start_clicked(GtkWidget *widget G_GNUC_UNUSED, gpointer user_data) {
    AppData *app = (AppData *)user_data;
    
    if (!app->is_counting) {
        int hours = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app->hour_spin));
        int minutes = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app->minute_spin));
        
        if (hours == 0 && minutes == 0) {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(app->window),
                                                      GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                      GTK_MESSAGE_WARNING,
                                                      GTK_BUTTONS_OK,
                                                      "Please set at least 1 minute for the timer.");
            gtk_window_present(GTK_WINDOW(dialog));
            g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_destroy), NULL);
            return;
        }
        
        app->remaining_seconds = (hours * 3600) + (minutes * 60);
        app->is_counting = TRUE;
        app->sound_played = FALSE;
        
        gtk_widget_set_sensitive(app->hour_spin, FALSE);
        gtk_widget_set_sensitive(app->minute_spin, FALSE);
        gtk_widget_set_sensitive(app->power_action_dropdown, FALSE);
        gtk_button_set_label(GTK_BUTTON(app->start_button), "Pause");
        gtk_widget_set_sensitive(app->cancel_button, TRUE);
        
        app->timer_id = g_timeout_add(1000, update_countdown, app);
        update_countdown(app);
    } else {
        g_source_remove(app->timer_id);
        app->is_counting = FALSE;
        gtk_button_set_label(GTK_BUTTON(app->start_button), "Resume");
    }
}

void on_cancel_clicked(GtkWidget *widget G_GNUC_UNUSED, gpointer user_data) {
    AppData *app = (AppData *)user_data;
    
    if (app->is_counting) {
        g_source_remove(app->timer_id);
    }
    
    app->is_counting = FALSE;
    app->remaining_seconds = 0;
    app->sound_played = FALSE;
    
    gtk_widget_set_sensitive(app->hour_spin, TRUE);
    gtk_widget_set_sensitive(app->minute_spin, TRUE);
    gtk_widget_set_sensitive(app->power_action_dropdown, TRUE);
    gtk_button_set_label(GTK_BUTTON(app->start_button), "Start");
    gtk_widget_set_sensitive(app->cancel_button, FALSE);
    gtk_label_set_text(GTK_LABEL(app->countdown_label), "00:00:00");
}

void on_window_destroy(GtkWidget *widget G_GNUC_UNUSED, gpointer user_data) {
    AppData *app = (AppData *)user_data;
    
    if (app->is_counting && app->timer_id > 0) {
        g_source_remove(app->timer_id);
    }
    
    g_free(app);
}