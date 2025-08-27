#include "timed_shutdown.h"

static void activate(AdwApplication *app, gpointer user_data G_GNUC_UNUSED) {
    AppData *app_data = g_new0(AppData, 1);
    
    create_ui(app_data, app);
    setup_keyboard_shortcuts(app_data);
    
    gtk_window_present(GTK_WINDOW(app_data->window));
}

int main(int argc, char **argv) {
    AdwApplication *app = adw_application_new("in.rahulbali.PowerTimer", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    
    return status;
}