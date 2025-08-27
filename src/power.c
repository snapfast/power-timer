#include "power_timer.h"

void execute_power_action(const char *action_type, const char *command) {
    int result = system(command);
    
    if (result != 0) {
        g_warning("Power action command failed with exit code: %d", result);
    }
}


void show_power_action_confirmation(GtkWidget *parent_window, const char *action_name, const char *command) {
    execute_power_action(action_name, command);
}