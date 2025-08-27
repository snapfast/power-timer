# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Application Overview

Power Timer is a GTK4/Libadwaita desktop application for Linux that schedules system power actions (shutdown, restart, log off) with a countdown timer. The application automatically executes power actions when timers expire without requiring user confirmation.

## Build System & Commands

### Development Build
```bash
./build.sh
```
This creates a development build in the `build/` directory and outputs the executable at `build/power-timer`.

### Release Build
```bash
./release.sh [version]
```
Creates a production release with optimized build settings (`--buildtype=release --strip`), packages it as a tarball with installation scripts, and generates SHA256 checksums in the `release/` directory.

### Manual Build Commands
```bash
# Setup build directory
meson setup build

# Build application  
cd build && ninja

# Install system-wide
sudo ninja install

# Clean rebuild
rm -rf build && meson setup build
```

### Running the Application
```bash
# Development build
./build/power-timer

# Installed version
power-timer
```

## Code Architecture

### Modular C Structure
The application follows a modular architecture with clear separation of concerns:

- **`main.c`**: Application entry point and Adwaita app initialization
- **`ui.c`**: Complete UI construction using GTK4/Libadwaita, CSS styling, and keyboard shortcuts
- **`timer.c`**: Core timer logic, countdown updates, and control flow (start/pause/cancel)
- **`power.c`**: System command execution for power actions (no confirmation dialogs)
- **`sound.c`**: System sound integration with fallback chain for cross-distribution compatibility
- **`power_timer.h`**: Shared AppData structure and function declarations

### Key Data Flow
1. **UI Setup**: `create_ui()` builds the interface and connects signals to timer control functions
2. **Timer Control**: `on_start_clicked()` initiates `g_timeout_add()` with `update_countdown()` callback
3. **Countdown Logic**: `update_countdown()` decrements timer, updates display, triggers sound alerts, and executes power actions automatically
4. **Power Execution**: `show_power_action_confirmation()` directly calls `execute_power_action()` without user prompts

### AppData Structure
Central state management through a single `AppData` struct containing all UI widgets, timer state (`remaining_seconds`, `is_counting`, `timer_id`), and control flags.

### System Integration
- **Power Commands**: Uses systemd commands (`systemctl poweroff/reboot`, `loginctl terminate-user`)
- **Sound System**: Attempts multiple sound backends with graceful fallbacks (canberra-gtk, pactl, pw-play, aplay, bell)
- **Desktop Integration**: Installs `.desktop` file and SVG icon to system directories

## Important Implementation Details

### Timer Mechanism
- Uses GTK's `g_timeout_add()` for 1-second interval updates
- Timer state persists in `AppData.remaining_seconds` 
- Sound notification triggered in final minute via `AppData.sound_played` flag
- Automatic cleanup via `g_source_remove()` on timer completion

### UI Styling
- Heavy use of CSS-in-JS through `gtk_css_provider_load_from_data()`
- Custom styling for countdown display (6em monospace font)
- Libadwaita components for modern GNOME appearance

### Memory Management
- Manual cleanup required for `AppData` structure on window destroy
- String duplication needed for async command execution contexts
- GTK reference counting handles UI widget lifecycle

### Release Packaging
The `release.sh` script creates complete distribution packages including:
- Optimized binary with debug symbols stripped
- Installation script with user/system-wide options
- All source files and build configuration
- Desktop file and icon assets