#include "power_timer.h"

char* get_sound_file_path(void) {
    return NULL;
}

void play_sound_notification(void) {
    char *sound_cmd = g_strdup(
        "(canberra-gtk-play -i bell 2>/dev/null || "
        "pactl play-sample bell 2>/dev/null || "
        "pw-play /usr/share/sounds/freedesktop/stereo/bell.oga 2>/dev/null || "
        "aplay /usr/share/sounds/alsa/Front_Left.wav 2>/dev/null || "
        "printf '\\a') && sleep 0.5 && "
        "(canberra-gtk-play -i bell 2>/dev/null || "
        "pactl play-sample bell 2>/dev/null || "
        "pw-play /usr/share/sounds/freedesktop/stereo/bell.oga 2>/dev/null || "
        "aplay /usr/share/sounds/alsa/Front_Left.wav 2>/dev/null || "
        "printf '\\a') && sleep 0.5 && "
        "(canberra-gtk-play -i bell 2>/dev/null || "
        "pactl play-sample bell 2>/dev/null || "
        "pw-play /usr/share/sounds/freedesktop/stereo/bell.oga 2>/dev/null || "
        "aplay /usr/share/sounds/alsa/Front_Left.wav 2>/dev/null || "
        "printf '\\a') || true");
        
    int result = system(sound_cmd);
    if (result != 0) {
        g_warning("Sound notification command failed with exit code: %d", result);
    }
    
    g_free(sound_cmd);
}