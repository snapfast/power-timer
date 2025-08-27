# Timed Shutdown

A modern GNOME application that schedules system shutdown with a countdown timer. Built with GTK4 and Libadwaita for a clean, native desktop experience.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Version](https://img.shields.io/badge/version-1.0.0-green.svg)
![GTK](https://img.shields.io/badge/GTK-4.x-orange.svg)

## ✨ Features

- **Modern UI**: Clean GTK4 + Libadwaita interface designed for GNOME
- **Flexible Timer**: Set shutdown time from 1 minute to 23 hours 59 minutes
- **Real-time Countdown**: Live HH:MM:SS display with large, easy-to-read text
- **Full Control**: Start, pause, resume, and cancel operations
- **Keyboard Shortcuts**: 
  - `Ctrl+Enter`: Start/pause timer
  - `Esc`: Cancel timer  
  - `Ctrl+Q/W`: Quit application
- **Safe Shutdown**: Uses `systemctl poweroff` for clean system shutdown

## 📦 Quick Install

### Download Pre-built Binary (Recommended)

1. **Download** the latest release from [GitHub Releases](https://github.com/snapfast/timed-shutdown/releases)
2. **Extract** and install:
   ```bash
   tar -xzf timed-shutdown-1.0.0-linux-x86_64.tar.gz
   cd timed-shutdown-1.0.0
   ./install.sh  # User install
   # OR
   sudo ./install.sh  # System-wide install
   ```
3. **Launch** from applications menu or run `timed-shutdown`

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
git clone https://github.com/snapfast/timed-shutdown.git
cd timed-shutdown
./build.sh
```

#### Install System-wide
```bash
cd build
sudo ninja install
```

## 🚀 Usage

### Basic Operation
1. **Set Time**: Use the hour and minute spinboxes to set your desired shutdown time
2. **Start Timer**: Click "Start" to begin the countdown
3. **Monitor**: Watch the real-time countdown in HH:MM:SS format
4. **Control**: Use "Pause"/"Resume" to pause/continue, or "Cancel" to abort

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

## 📋 System Requirements

- **OS**: Linux with systemd
- **Desktop**: GNOME (recommended) or any GTK4-compatible environment  
- **Permissions**: Ability to execute `systemctl poweroff`
- **Architecture**: x86_64 (pre-built binaries)

## 🔒 Security & Permissions

This application executes `systemctl poweroff` to shutdown your system. Ensure you have appropriate permissions:

- **User install**: Your user account should be able to shutdown the system
- **Groups**: You may need to be in `sudo` or `wheel` group depending on your distribution
- **PolicyKit**: Modern systems use PolicyKit for shutdown permissions

## 🛠️ Development

### Creating Releases
```bash
./release.sh [version]  # Creates distributable package
```

### Project Structure
```
timed-shutdown/
├── src/main.c           # Main application code
├── data/               # Desktop file and resources
├── meson.build         # Build configuration  
├── build.sh           # Development build script
├── release.sh         # Release packaging script
└── README.md          # This file
```

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 🐛 Issues

Report bugs and request features on [GitHub Issues](https://github.com/snapfast/timed-shutdown/issues).
