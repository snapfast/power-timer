#include "timed_shutdown.h"

void execute_power_action(const char *action_type, const char *command) {
    int result = system(command);
    
    if (result != 0) {
        g_warning("Power action command failed with exit code: %d", result);
    }
}

static void on_power_action_response(GtkDialog *dialog, int response_id, gpointer user_data) {
    char *command = (char *)user_data;
    
    if (response_id == GTK_RESPONSE_YES) {
        execute_power_action("", command);
    }
    
    gtk_window_destroy(GTK_WINDOW(dialog));
    g_free(command);
}

void show_power_action_confirmation(GtkWidget *parent_window, const char *action_name, const char *command) {
    char *message = g_strdup_printf("The timer has expired.\n\nProceed with %s?", action_name);
    
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent_window),
                                              GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                              GTK_MESSAGE_QUESTION,
                                              GTK_BUTTONS_YES_NO,
                                              "%s", message);
    
    gtk_window_set_title(GTK_WINDOW(dialog), "Confirm Action");
    
    char *command_copy = g_strdup(command);
    g_signal_connect(dialog, "response", G_CALLBACK(on_power_action_response), command_copy);
    
    gtk_window_present(GTK_WINDOW(dialog));
    
    g_free(message);
}