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
    GtkWidget *sound_switch;
    GtkWidget *action_view_stack;
    GtkWidget *action_switcher;
    guint timer_id;
    int remaining_seconds;
    gboolean is_counting;
    gboolean sound_played;
} AppData;

static gboolean update_countdown(gpointer user_data) {
    AppData *app = (AppData *)user_data;
    
    if (app->remaining_seconds <= 0) {
        // Get selected action from view switcher
        const char *selected_page = adw_view_stack_get_visible_child_name(ADW_VIEW_STACK(app->action_view_stack));
        if (g_strcmp0(selected_page, "shutdown") == 0) {
            system("systemctl poweroff");
        } else if (g_strcmp0(selected_page, "restart") == 0) {
            system("systemctl reboot");
        } else if (g_strcmp0(selected_page, "logoff") == 0) {
            system("loginctl terminate-user $USER");
        } else {
            system("systemctl poweroff"); // Default fallback
        }
        return G_SOURCE_REMOVE;
    }
    
    // Play sound notification in the last minute (60 seconds) if enabled
    if (app->remaining_seconds <= 60 && !app->sound_played && 
        gtk_switch_get_active(GTK_SWITCH(app->sound_switch))) {
        // Play bird sound notification 3 times for emphasis
        system("(paplay data/mixkit-forest-birds-singing-1212.wav 2>/dev/null || "
               "aplay data/mixkit-forest-birds-singing-1212.wav 2>/dev/null || "
               "mpv --no-video --volume=50 data/mixkit-forest-birds-singing-1212.wav 2>/dev/null || "
               "printf '\\a') && sleep 0.5 && "
               "(paplay data/mixkit-forest-birds-singing-1212.wav 2>/dev/null || "
               "aplay data/mixkit-forest-birds-singing-1212.wav 2>/dev/null || "
               "mpv --no-video --volume=50 data/mixkit-forest-birds-singing-1212.wav 2>/dev/null || "
               "printf '\\a') && sleep 0.5 && "
               "(paplay data/mixkit-forest-birds-singing-1212.wav 2>/dev/null || "
               "aplay data/mixkit-forest-birds-singing-1212.wav 2>/dev/null || "
               "mpv --no-video --volume=50 data/mixkit-forest-birds-singing-1212.wav 2>/dev/null || "
               "printf '\\a') || true");
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

static void on_start_clicked(GtkWidget *widget G_GNUC_UNUSED, gpointer user_data) {
    AppData *app = (AppData *)user_data;
    
    if (!app->is_counting) {
        int hours = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app->hour_spin));
        int minutes = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app->minute_spin));
        app->remaining_seconds = (hours * 3600) + (minutes * 60);
        app->is_counting = TRUE;
        app->sound_played = FALSE;
        
        gtk_widget_set_sensitive(app->hour_spin, FALSE);
        gtk_widget_set_sensitive(app->minute_spin, FALSE);
        gtk_widget_set_sensitive(app->action_switcher, FALSE);
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

static void on_cancel_clicked(GtkWidget *widget G_GNUC_UNUSED, gpointer user_data) {
    AppData *app = (AppData *)user_data;
    
    if (app->is_counting) {
        g_source_remove(app->timer_id);
    }
    
    app->is_counting = FALSE;
    app->remaining_seconds = 0;
    app->sound_played = FALSE;
    
    gtk_widget_set_sensitive(app->hour_spin, TRUE);
    gtk_widget_set_sensitive(app->minute_spin, TRUE);
    gtk_widget_set_sensitive(app->action_switcher, TRUE);
    gtk_button_set_label(GTK_BUTTON(app->start_button), "Start");
    gtk_widget_set_sensitive(app->cancel_button, FALSE);
    gtk_label_set_text(GTK_LABEL(app->countdown_label), "00:00:00");
}

static gboolean on_key_pressed(GtkEventControllerKey *controller G_GNUC_UNUSED,
                              guint keyval, guint keycode G_GNUC_UNUSED, 
                              GdkModifierType state, gpointer user_data) {
    AppData *app = (AppData *)user_data;
    
    if (state & GDK_CONTROL_MASK) {
        switch (keyval) {
            case GDK_KEY_Return:
            case GDK_KEY_KP_Enter:
                on_start_clicked(NULL, app);
                return TRUE;
            case GDK_KEY_Escape:
                if (app->is_counting) {
                    on_cancel_clicked(NULL, app);
                }
                return TRUE;
            case GDK_KEY_q:
            case GDK_KEY_w:
                gtk_window_close(GTK_WINDOW(app->window));
                return TRUE;
        }
    } else if (keyval == GDK_KEY_Escape && app->is_counting) {
        on_cancel_clicked(NULL, app);
        return TRUE;
    }
    
    return FALSE;
}

static void activate(AdwApplication *app, gpointer user_data G_GNUC_UNUSED) {
    AppData *app_data = g_new0(AppData, 1);
    
    app_data->window = adw_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(app_data->window), "Timed Shutdown");
    gtk_window_set_default_size(GTK_WINDOW(app_data->window), 640, 480);
    gtk_window_set_resizable(GTK_WINDOW(app_data->window), TRUE);
    
    GtkWidget *header_bar = adw_header_bar_new();
    
    GtkWidget *window_content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(window_content), header_bar);
    
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 36);
    gtk_widget_set_margin_top(main_box, 36);
    gtk_widget_set_margin_bottom(main_box, 36);
    gtk_widget_set_margin_start(main_box, 36);
    gtk_widget_set_margin_end(main_box, 36);
    
    gtk_box_append(GTK_BOX(window_content), main_box);
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(app_data->window), window_content);
    
    GtkWidget *time_group = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(time_group), "Shutdown Timer");
    adw_preferences_group_set_description(ADW_PREFERENCES_GROUP(time_group), "Schedule automatic system shutdown");
    
    GtkWidget *hour_row = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(hour_row), "Hours");
    
    app_data->hour_spin = gtk_spin_button_new_with_range(0, 23, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_data->hour_spin), 0);
    gtk_widget_set_size_request(app_data->hour_spin, 120, 50);
    gtk_widget_set_valign(app_data->hour_spin, GTK_ALIGN_CENTER);
    adw_action_row_add_suffix(ADW_ACTION_ROW(hour_row), app_data->hour_spin);
    
    GtkWidget *minute_row = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(minute_row), "Minutes");
    
    app_data->minute_spin = gtk_spin_button_new_with_range(0, 59, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_data->minute_spin), 30);
    gtk_widget_set_size_request(app_data->minute_spin, 120, 50);
    gtk_widget_set_valign(app_data->minute_spin, GTK_ALIGN_CENTER);
    adw_action_row_add_suffix(ADW_ACTION_ROW(minute_row), app_data->minute_spin);
    
    GtkWidget *action_group = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(action_group), "Power Action");
    
    app_data->action_view_stack = adw_view_stack_new();
    app_data->action_switcher = adw_view_switcher_new();
    adw_view_switcher_set_stack(ADW_VIEW_SWITCHER(app_data->action_switcher), ADW_VIEW_STACK(app_data->action_view_stack));
    
    // Add pages to the view stack (these are just placeholders)
    GtkWidget *shutdown_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *restart_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *logoff_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    
    adw_view_stack_add_titled(ADW_VIEW_STACK(app_data->action_view_stack), shutdown_page, "shutdown", "Shutdown");
    adw_view_stack_add_titled(ADW_VIEW_STACK(app_data->action_view_stack), restart_page, "restart", "Restart");
    adw_view_stack_add_titled(ADW_VIEW_STACK(app_data->action_view_stack), logoff_page, "logoff", "Log off");
    
    adw_view_stack_set_visible_child_name(ADW_VIEW_STACK(app_data->action_view_stack), "shutdown"); // Default to shutdown
    
    gtk_widget_set_halign(app_data->action_switcher, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(app_data->action_switcher, 12);
    gtk_widget_set_margin_bottom(app_data->action_switcher, 12);
    
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(action_group), app_data->action_switcher);
    
    GtkWidget *sound_row = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(sound_row), "Timer Ending Alert");
    adw_action_row_set_subtitle(ADW_ACTION_ROW(sound_row), "Play sound notification in the last minute");
    
    app_data->sound_switch = gtk_switch_new();
    gtk_switch_set_active(GTK_SWITCH(app_data->sound_switch), TRUE);
    gtk_widget_set_valign(app_data->sound_switch, GTK_ALIGN_CENTER);
    adw_action_row_add_suffix(ADW_ACTION_ROW(sound_row), app_data->sound_switch);
    
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(time_group), hour_row);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(time_group), minute_row);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(time_group), sound_row);
    gtk_box_append(GTK_BOX(main_box), time_group);
    
    gtk_box_append(GTK_BOX(main_box), action_group);
    
    GtkWidget *countdown_group = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(countdown_group), "Countdown");
    
    app_data->countdown_label = gtk_label_new("00:00:00");
    gtk_widget_add_css_class(app_data->countdown_label, "display-1");
    gtk_widget_add_css_class(app_data->countdown_label, "numeric");
    gtk_widget_set_halign(app_data->countdown_label, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(app_data->countdown_label, 40);
    gtk_widget_set_margin_bottom(app_data->countdown_label, 40);
    
    // Make all text larger with custom CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, 
        ".display-1 { font-size: 4em; font-weight: bold; }\n"
        "button { font-size: 1.2em; font-weight: 500; }\n"
        "spinbutton { font-size: 1.3em; }\n"
        ".preferences-group > .header .title { font-size: 1.3em; font-weight: 600; }\n"
        ".preferences-group > .header .description { font-size: 1.1em; }\n"
        "row.activatable .title { font-size: 1.2em; font-weight: 500; }\n"
        "headerbar { min-height: 60px; }\n"
        "headerbar .title { font-size: 1.3em; font-weight: 600; }", -1);
    gtk_style_context_add_provider_for_display(
        gtk_widget_get_display(app_data->countdown_label),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    GtkWidget *countdown_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(countdown_box), app_data->countdown_label);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(countdown_group), countdown_box);
    gtk_box_append(GTK_BOX(main_box), countdown_group);
    
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 24);
    gtk_widget_set_halign(button_box, GTK_ALIGN_CENTER);
    gtk_box_set_homogeneous(GTK_BOX(button_box), TRUE);
    
    app_data->start_button = gtk_button_new_with_label("Start");
    gtk_widget_add_css_class(app_data->start_button, "suggested-action");
    gtk_widget_add_css_class(app_data->start_button, "pill");
    gtk_widget_set_size_request(app_data->start_button, 160, 60);
    
    app_data->cancel_button = gtk_button_new_with_label("Cancel");
    gtk_widget_add_css_class(app_data->cancel_button, "destructive-action");
    gtk_widget_add_css_class(app_data->cancel_button, "pill");
    gtk_widget_set_size_request(app_data->cancel_button, 160, 60);
    gtk_widget_set_sensitive(app_data->cancel_button, FALSE);
    
    gtk_box_append(GTK_BOX(button_box), app_data->start_button);
    gtk_box_append(GTK_BOX(button_box), app_data->cancel_button);
    gtk_box_append(GTK_BOX(main_box), button_box);
    
    g_signal_connect(app_data->start_button, "clicked", G_CALLBACK(on_start_clicked), app_data);
    g_signal_connect(app_data->cancel_button, "clicked", G_CALLBACK(on_cancel_clicked), app_data);
    
    // Keyboard shortcuts
    GtkEventController *key_controller = gtk_event_controller_key_new();
    g_signal_connect(key_controller, "key-pressed", G_CALLBACK(on_key_pressed), app_data);
    gtk_widget_add_controller(app_data->window, key_controller);
    
    app_data->is_counting = FALSE;
    app_data->sound_played = FALSE;
    
    gtk_window_present(GTK_WINDOW(app_data->window));
}

int main(int argc, char **argv) {
    AdwApplication *app = adw_application_new("com.example.TimedShutdown", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    
    return status;
}