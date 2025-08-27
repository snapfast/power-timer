# Timed Shutdown

A simple GNOME application that shuts down the computer after a selected time with a countdown timer.

## Features

- Modern GTK4 + Libadwaita UI
- Time selection from 1 to 1440 minutes (24 hours)
- Real-time countdown display
- Start/Pause/Cancel functionality
- Automatic system shutdown when timer reaches zero

## Dependencies

- GTK4
- Libadwaita
- Meson (build system)
- Ninja (build tool)

## Installation

### On Ubuntu/Debian:
```bash
sudo apt install libgtk-4-dev libadwaita-1-dev meson ninja-build
```

### On Fedora:
```bash
sudo dnf install gtk4-devel libadwaita-devel meson ninja-build
```

## Building

1. Clone or download the source code
2. Run the build script:
```bash
./build.sh
```

3. Run the application:
```bash
./build/timed-shutdown
```

## Installing System-wide

After building:
```bash
cd build
sudo ninja install
```

The application will be available in your applications menu as "Timed Shutdown".

## Usage

1. Set the desired time in minutes using the spin button
2. Click "Start" to begin the countdown
3. The countdown will display in HH:MM:SS format
4. Click "Pause" to pause the timer, "Resume" to continue
5. Click "Cancel" to stop and reset the timer
6. The system will automatically shutdown when the timer reaches zero

## Security Note

This application requires permission to shutdown the system. Make sure you have the necessary privileges or are part of the appropriate groups to execute shutdown commands.# timed-shutdown
