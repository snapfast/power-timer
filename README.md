# Power Timer

<div align="center">
  <img src="data/power-timer.svg" width="128" height="128" alt="Power Timer Logo">
</div>

A modern GNOME application that schedules system power actions with a countdown timer. Built with GTK4 and Libadwaita for a clean, native desktop experience.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Version](https://img.shields.io/badge/version-2.1.2-green.svg)
![GTK](https://img.shields.io/badge/GTK-4.x-orange.svg)

![Power Timer Screenshot](data/Screenshot%20from%202025-08-27%2022-35-02.png)

## Features

- **Modern Dropdown UI**: Clean GTK4 + Libadwaita interface with modern dropdown selection
- **Multiple Power Actions**: Choose between shutdown, restart, or log off with dropdown selection
- **Flexible Timer**: Set timer from 1 minute to 23 hours 59 minutes with horizontal layout
- **Large Countdown Display**: Live HH:MM:SS display with 6em monospace font (JetBrains Mono/Fira Code)
- **Full Control**: Start, pause, resume, and cancel operations
- **System Sound Integration**: Automatic sound notifications using system sounds (no custom files)
- **Automatic Execution**: Power actions execute automatically when timer expires (no confirmation required)
- **Input Validation**: Prevents invalid timer settings (e.g., 0 hours and 0 minutes)
- **Keyboard Shortcuts**: 
  - `Ctrl+Enter`: Start/pause timer
  - `Esc`: Cancel timer  
  - `Ctrl+Q/W`: Quit application
- **Safe Operations**: Uses systemd commands (`systemctl`, `loginctl`) for clean system actions
- **Compact Interface**: Reduced spacing and 480px width for efficient screen usage

## Quick Install

### Download Pre-built Binary (Recommended)

1. **Download** the latest release from [GitHub Releases](https://github.com/snapfast/power-timer/releases)
2. **Extract** and install:
   ```bash
   tar -xzf power-timer-2.1.2-linux-x86_64.tar.gz
   cd power-timer-2.1.2
   ./install.sh  # User install
   # OR
   sudo ./install.sh  # System-wide install
   ```
3. **Launch** from applications menu or run `power-timer`

### Build from Source

#### Dependencies
- **Runtime**: GTK4, Libadwaita
- **Build**: Meson, Ninja, GCC

#### Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt install libgtk-4-dev libadwaita-1-dev meson ninja-build
```

**Fedora:**
```bash
sudo dnf install gtk4-devel libadwaita-devel meson ninja-build
```

#### Build Steps
```bash
git clone https://github.com/snapfast/power-timer.git
cd power-timer
./build.sh
```

## Usage

### Basic Operation
1. **Set Time**: Use the horizontal hour and minute controls to set your desired timer duration
2. **Choose Action**: Select shutdown, restart, or log off from the dropdown menu
3. **Start Timer**: Click "Start" to begin the countdown
4. **Monitor**: Watch the large real-time countdown in HH:MM:SS format with monospace font
5. **Control**: Use "Pause"/"Resume" to pause/continue, or "Cancel" to abort
6. **Sound Alert**: Automatic system sound notification in the last minute
7. **Automatic Action**: When timer expires, the selected power action executes automatically

### Keyboard Shortcuts
| Shortcut | Action |
|----------|--------|
| `Ctrl+Enter` | Start or pause/resume timer |
| `Esc` | Cancel running timer |
| `Ctrl+Q` or `Ctrl+W` | Quit application |

### Example Usage
- **Quick nap**: Set 30 minutes for a power nap
- **Downloads**: Set 2 hours for large file downloads
- **Rendering**: Set 8 hours for overnight video rendering
- **Energy saving**: Auto-shutdown after work hours

## System Requirements

- **OS**: Linux with systemd
- **Desktop**: GNOME (recommended) or any GTK4-compatible environment  
- **Permissions**: Ability to execute `systemctl poweroff`
- **Architecture**: x86_64 (pre-built binaries)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Issues

Report bugs and request features on [GitHub Issues](https://github.com/snapfast/power-timer/issues).