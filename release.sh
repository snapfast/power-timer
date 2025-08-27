#!/bin/bash

# Release build script for Power Timer application
# Creates distributable binaries for GitHub releases

set -e

VERSION=${1:-"1.0.0"}
APP_NAME="power-timer"
RELEASE_DIR="release"

echo "Building Power Timer v${VERSION} for release..."

# Clean up previous builds
if [ -d "build" ]; then
    rm -rf build
fi

if [ -d "$RELEASE_DIR" ]; then
    rm -rf "$RELEASE_DIR"
fi

# Create directories
mkdir -p build
mkdir -p "$RELEASE_DIR"

cd build

# Configure with meson for release
meson setup . .. --buildtype=release --strip

# Build the application
ninja

echo "Build complete. Creating release package..."

# Create release directory structure
cd ..
mkdir -p "$RELEASE_DIR/$APP_NAME-$VERSION"

# Copy binary
cp build/$APP_NAME "$RELEASE_DIR/$APP_NAME-$VERSION/"

# Copy documentation and metadata
cp README.md "$RELEASE_DIR/$APP_NAME-$VERSION/"
cp data/in.rahulbali.PowerTimer.desktop "$RELEASE_DIR/$APP_NAME-$VERSION/"
cp build.sh "$RELEASE_DIR/$APP_NAME-$VERSION/"
cp meson.build "$RELEASE_DIR/$APP_NAME-$VERSION/"
cp -r src "$RELEASE_DIR/$APP_NAME-$VERSION/"
cp -r data "$RELEASE_DIR/$APP_NAME-$VERSION/"

# Create installation script
cat > "$RELEASE_DIR/$APP_NAME-$VERSION/install.sh" << 'EOF'
#!/bin/bash

# Installation script for Power Timer

set -e

# Application variables
APP_NAME="power-timer"
DESKTOP_ID="in.rahulbali.PowerTimer"
ICON_NAME="power-timer.svg"

echo "Installing Power Timer..."

# Check if running as root for system-wide installation
if [ "$EUID" -eq 0 ]; then
    # System-wide installation
    BIN_DIR="/usr/local/bin"
    DESKTOP_DIR="/usr/share/applications"
    ICON_DIR="/usr/share/icons/hicolor/scalable/apps"
    echo "Installing system-wide to $BIN_DIR"
else
    # User installation
    BIN_DIR="$HOME/.local/bin"
    DESKTOP_DIR="$HOME/.local/share/applications"
    ICON_DIR="$HOME/.local/share/icons/hicolor/scalable/apps"
    echo "Installing for current user to $BIN_DIR"
    
    # Create directories if they don't exist
    mkdir -p "$BIN_DIR"
    mkdir -p "$DESKTOP_DIR"
    mkdir -p "$ICON_DIR"
fi

# Copy binary
cp "$APP_NAME" "$BIN_DIR/"
chmod +x "$BIN_DIR/$APP_NAME"

# Create desktop file with full path for desktop app integration
cat > "$DESKTOP_DIR/$DESKTOP_ID.desktop" << DESKTOP_EOF
[Desktop Entry]
Name=Power Timer
Comment=Schedule system power actions with countdown timer
Exec=$BIN_DIR/$APP_NAME
Icon=power-timer
Terminal=false
Type=Application
Categories=System;Utility;
Keywords=power;timer;shutdown;restart;schedule;
DESKTOP_EOF

# Copy icon file
cp "data/$ICON_NAME" "$ICON_DIR/"

# Ensure hicolor theme index exists for icon cache
if [ ! -f "${ICON_DIR%/scalable/apps}/index.theme" ] && [ -f "/usr/share/icons/hicolor/index.theme" ]; then
    cp "/usr/share/icons/hicolor/index.theme" "${ICON_DIR%/scalable/apps}/"
fi

# Update desktop database
if command -v update-desktop-database >/dev/null 2>&1; then
    update-desktop-database "$DESKTOP_DIR" 2>/dev/null || true
fi

# Update icon cache
if command -v gtk-update-icon-cache >/dev/null 2>&1; then
    gtk-update-icon-cache -f "${ICON_DIR%/scalable/apps}" 2>/dev/null || true
fi

echo ""
echo "Desktop Application Installed Successfully!"
echo ""
EOF

chmod +x "$RELEASE_DIR/$APP_NAME-$VERSION/install.sh"

# Create uninstall script
cat > "$RELEASE_DIR/$APP_NAME-$VERSION/uninstall.sh" << 'EOF'
#!/bin/bash

# Power Timer Uninstall Script
set -e

# Application variables (must match install.sh)
APP_NAME="power-timer"
DESKTOP_ID="in.rahulbali.PowerTimer"
ICON_NAME="power-timer.svg"

echo "Removing Power Timer..."
read -p "Are you sure? [y/N]: " -n 1 -r
echo
[[ ! $REPLY =~ ^[Yy]$ ]] && { echo "Cancelled."; exit 0; }

# Check installation locations
for install_type in "system" "user"; do
    if [ "$install_type" = "system" ]; then
        BIN_DIR="/usr/local/bin"
        DESKTOP_DIR="/usr/share/applications"
        ICON_DIR="/usr/share/icons/hicolor/scalable/apps"
        USE_SUDO="sudo"
    else
        BIN_DIR="$HOME/.local/bin"
        DESKTOP_DIR="$HOME/.local/share/applications"
        ICON_DIR="$HOME/.local/share/icons/hicolor/scalable/apps"
        USE_SUDO=""
    fi
    
    binary="$BIN_DIR/$APP_NAME"
    desktop="$DESKTOP_DIR/$DESKTOP_ID.desktop"
    icon="$ICON_DIR/$ICON_NAME"
    
    if [[ -f "$binary" || -f "$desktop" || -f "$icon" ]]; then
        echo "Found $install_type installation"
        
        # Remove files
        $USE_SUDO rm -f "$binary" "$desktop" "$icon"
        
        # Update caches
        $USE_SUDO gtk-update-icon-cache -f "${ICON_DIR%/scalable/apps}" 2>/dev/null || true
        $USE_SUDO update-desktop-database "$DESKTOP_DIR" 2>/dev/null || true
        
        echo "Uninstalled $install_type installation"
    fi
done

echo "Power Timer removed successfully."
EOF

chmod +x "$RELEASE_DIR/$APP_NAME-$VERSION/uninstall.sh"

# Create tarball
cd "$RELEASE_DIR"
tar -czf "$APP_NAME-$VERSION-linux-x86_64.tar.gz" "$APP_NAME-$VERSION/"

# Create checksums
sha256sum "$APP_NAME-$VERSION-linux-x86_64.tar.gz" > "$APP_NAME-$VERSION-linux-x86_64.tar.gz.sha256"

cd ..

echo ""
echo "Release package created successfully!"
echo "Files in $RELEASE_DIR/:"
ls -la "$RELEASE_DIR/"
echo ""
echo "Ready for GitHub release upload:"
echo "- $RELEASE_DIR/$APP_NAME-$VERSION-linux-x86_64.tar.gz"
echo "- $RELEASE_DIR/$APP_NAME-$VERSION-linux-x86_64.tar.gz.sha256"