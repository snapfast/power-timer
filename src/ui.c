#include "power_timer.h"

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

void setup_keyboard_shortcuts(AppData *app_data) {
    GtkEventController *key_controller = gtk_event_controller_key_new();
    g_signal_connect(key_controller, "key-pressed", G_CALLBACK(on_key_pressed), app_data);
    gtk_widget_add_controller(app_data->window, key_controller);
}

GtkWidget* create_ui(AppData *app_data, AdwApplication *app) {
    app_data->window = adw_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(app_data->window), "Power Timer");
    gtk_window_set_default_size(GTK_WINDOW(app_data->window), 480, 480);
    gtk_window_set_resizable(GTK_WINDOW(app_data->window), TRUE);
    
    GtkWidget *header_bar = adw_header_bar_new();
    
    GtkWidget *window_content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(window_content), header_bar);
    
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);
    gtk_widget_set_margin_top(main_box, 24);
    gtk_widget_set_margin_bottom(main_box, 24);
    gtk_widget_set_margin_start(main_box, 24);
    gtk_widget_set_margin_end(main_box, 24);
    
    gtk_box_append(GTK_BOX(window_content), main_box);
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(app_data->window), window_content);
    
    GtkWidget *time_group = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(time_group), "Timer Settings");
    adw_preferences_group_set_description(ADW_PREFERENCES_GROUP(time_group), "Schedule automatic power actions");
    
    GtkWidget *time_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_margin_top(time_box, 8);
    gtk_widget_set_margin_bottom(time_box, 8);
    gtk_widget_set_margin_start(time_box, 12);
    gtk_widget_set_margin_end(time_box, 12);
    gtk_box_set_homogeneous(GTK_BOX(time_box), TRUE);
    
    GtkWidget *hour_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    GtkWidget *hour_label = gtk_label_new("Hours:");
    gtk_widget_set_halign(hour_label, GTK_ALIGN_START);
    app_data->hour_spin = gtk_spin_button_new_with_range(0, 23, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_data->hour_spin), 0);
    gtk_widget_set_hexpand(app_data->hour_spin, TRUE);
    gtk_widget_set_size_request(app_data->hour_spin, -1, 50);
    gtk_box_append(GTK_BOX(hour_box), hour_label);
    gtk_box_append(GTK_BOX(hour_box), app_data->hour_spin);
    
    GtkWidget *minute_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    GtkWidget *minute_label = gtk_label_new("Minutes:");
    gtk_widget_set_halign(minute_label, GTK_ALIGN_START);
    app_data->minute_spin = gtk_spin_button_new_with_range(0, 59, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(app_data->minute_spin), 30);
    gtk_widget_set_hexpand(app_data->minute_spin, TRUE);
    gtk_widget_set_size_request(app_data->minute_spin, -1, 50);
    gtk_box_append(GTK_BOX(minute_box), minute_label);
    gtk_box_append(GTK_BOX(minute_box), app_data->minute_spin);
    
    gtk_box_append(GTK_BOX(time_box), hour_box);
    gtk_box_append(GTK_BOX(time_box), minute_box);
    
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(time_group), time_box);
    
    GtkWidget *action_group = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(action_group), "Power Action");
    
    GtkWidget *action_row = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(action_row), "Action");
    adw_action_row_set_subtitle(ADW_ACTION_ROW(action_row), "Choose what happens when timer expires");
    
    const char *power_actions[] = {"Shutdown", "Restart", "Log off", NULL};
    GtkStringList *string_list = gtk_string_list_new(power_actions);
    app_data->power_action_dropdown = gtk_drop_down_new(G_LIST_MODEL(string_list), NULL);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(app_data->power_action_dropdown), 0);
    gtk_widget_set_valign(app_data->power_action_dropdown, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(app_data->power_action_dropdown, 140, 40);
    adw_action_row_add_suffix(ADW_ACTION_ROW(action_row), app_data->power_action_dropdown);
    
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(action_group), action_row);
    
    
    gtk_box_append(GTK_BOX(main_box), time_group);
    
    gtk_box_append(GTK_BOX(main_box), action_group);
    
    GtkWidget *countdown_group = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(countdown_group), "Countdown");
    
    app_data->countdown_label = gtk_label_new("00:00:00");
    gtk_widget_add_css_class(app_data->countdown_label, "display-1");
    gtk_widget_add_css_class(app_data->countdown_label, "numeric");
    gtk_widget_set_halign(app_data->countdown_label, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(app_data->countdown_label, 20);
    gtk_widget_set_margin_bottom(app_data->countdown_label, 20);
    
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, 
        ".display-1 { font-family: 'JetBrains Mono', 'Fira Code', 'Source Code Pro', 'Courier New', monospace; font-size: 6em; font-weight: bold; color: #1a1a1a; }\n"
        "button { font-size: 1.2em; font-weight: 500; }\n"
        "spinbutton { font-size: 1.3em; }\n"
        ".preferences-group > .header .title { font-size: 1.3em; font-weight: 600; color: #1a1a1a; }\n"
        ".preferences-group > .header .description { font-size: 1.1em; color: #1a1a1a; }\n"
        "row.activatable .title { font-size: 1.2em; font-weight: 500; color: #1a1a1a; }\n"
        "headerbar { min-height: 60px; }\n"
        "headerbar .title { font-size: 1.3em; font-weight: 600; color: #1a1a1a; }\n"
        "label { color: #1a1a1a; }", -1);
    gtk_style_context_add_provider_for_display(
        gtk_widget_get_display(app_data->countdown_label),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    GtkWidget *countdown_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(countdown_box), app_data->countdown_label);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(countdown_group), countdown_box);
    gtk_box_append(GTK_BOX(main_box), countdown_group);
    
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
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
    
    GtkWidget *warning_label = gtk_label_new("Closing this application will stop the timer and cancel any scheduled actions.");
    gtk_widget_add_css_class(warning_label, "dim-label");
    gtk_widget_add_css_class(warning_label, "caption");
    gtk_label_set_wrap(GTK_LABEL(warning_label), TRUE);
    gtk_label_set_justify(GTK_LABEL(warning_label), GTK_JUSTIFY_CENTER);
    gtk_widget_set_halign(warning_label, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(warning_label, 16);
    gtk_widget_set_margin_start(warning_label, 12);
    gtk_widget_set_margin_end(warning_label, 12);
    gtk_box_append(GTK_BOX(main_box), warning_label);
    
    g_signal_connect(app_data->start_button, "clicked", G_CALLBACK(on_start_clicked), app_data);
    g_signal_connect(app_data->cancel_button, "clicked", G_CALLBACK(on_cancel_clicked), app_data);
    g_signal_connect(app_data->window, "destroy", G_CALLBACK(on_window_destroy), app_data);
    
    app_data->is_counting = FALSE;
    app_data->sound_played = FALSE;
    
    return app_data->window;
}