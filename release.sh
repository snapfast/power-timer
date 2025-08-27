#!/bin/bash

# Release build script for Timed Shutdown application
# Creates distributable binaries for GitHub releases

set -e

VERSION=${1:-"1.0.0"}
APP_NAME="timed-shutdown"
RELEASE_DIR="release"

echo "Building ${APP_NAME} v${VERSION} for release..."

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
cp data/com.example.TimedShutdown.desktop "$RELEASE_DIR/$APP_NAME-$VERSION/"
cp build.sh "$RELEASE_DIR/$APP_NAME-$VERSION/"
cp meson.build "$RELEASE_DIR/$APP_NAME-$VERSION/"
cp -r src "$RELEASE_DIR/$APP_NAME-$VERSION/"
cp -r data "$RELEASE_DIR/$APP_NAME-$VERSION/"

# Create installation script
cat > "$RELEASE_DIR/$APP_NAME-$VERSION/install.sh" << 'EOF'
#!/bin/bash

# Installation script for Timed Shutdown

set -e

APP_NAME="timed-shutdown"
DESKTOP_FILE="data/com.example.TimedShutdown.desktop"

echo "Installing Timed Shutdown..."

# Check if running as root for system-wide installation
if [ "$EUID" -eq 0 ]; then
    # System-wide installation
    INSTALL_DIR="/usr/local/bin"
    DESKTOP_DIR="/usr/share/applications"
    ICON_DIR="/usr/share/icons/hicolor/scalable/apps"
    echo "Installing system-wide to $INSTALL_DIR"
else
    # User installation
    INSTALL_DIR="$HOME/.local/bin"
    DESKTOP_DIR="$HOME/.local/share/applications"
    ICON_DIR="$HOME/.local/share/icons/hicolor/scalable/apps"
    echo "Installing for current user to $INSTALL_DIR"
    
    # Create directories if they don't exist
    mkdir -p "$INSTALL_DIR"
    mkdir -p "$DESKTOP_DIR"
    mkdir -p "$ICON_DIR"
fi

# Copy binary
cp "$APP_NAME" "$INSTALL_DIR/"
chmod +x "$INSTALL_DIR/$APP_NAME"

# Create desktop file with full path for desktop app integration
cat > "$DESKTOP_DIR/com.example.TimedShutdown.desktop" << EOF
[Desktop Entry]
Name=Timed Shutdown
Comment=Schedule system shutdown with countdown timer
Exec=$INSTALL_DIR/$APP_NAME
Icon=timed-shutdown
Terminal=false
Type=Application
Categories=System;Utility;
Keywords=shutdown;timer;schedule;
EOF

# Copy icon file
cp "data/timed-shutdown.svg" "$ICON_DIR/"

# Update desktop database
if command -v update-desktop-database >/dev/null 2>&1; then
    if [ "$EUID" -eq 0 ]; then
        update-desktop-database /usr/share/applications
    else
        update-desktop-database "$HOME/.local/share/applications"
    fi
fi

# Update icon cache
if command -v gtk-update-icon-cache >/dev/null 2>&1; then
    if [ "$EUID" -eq 0 ]; then
        gtk-update-icon-cache -f /usr/share/icons/hicolor
    else
        gtk-update-icon-cache -f "$HOME/.local/share/icons/hicolor" 2>/dev/null || true
    fi
fi

echo ""
echo "✅ Desktop Application Installed Successfully!"
echo ""
echo "📱 Launch the app:"
echo "   1. Press Super key (Windows key) and search 'Timed Shutdown'"
echo "   2. Or look in System/Utilities in your application menu"
echo "   3. Click the app with the 8-bit power/clock icon"
echo ""
echo "🖥️  Desktop Integration:"
echo "   - App appears in applications menu with custom icon"
echo "   - Launches as native desktop application"
echo "   - No terminal window required"
echo ""
if [ "$EUID" -ne 0 ]; then
echo "💡 Optional: Add to PATH for command line use:"
echo "   echo 'export PATH=\"\$HOME/.local/bin:\$PATH\"' >> ~/.bashrc"
echo "   source ~/.bashrc"
echo ""
fi
echo "🔧 Troubleshooting:"
echo "   - If app not visible: Log out and back in"
echo "   - Manual refresh: update-desktop-database $DESKTOP_DIR"
echo "   - Icon issues: gtk-update-icon-cache $ICON_DIR/../.."
EOF

chmod +x "$RELEASE_DIR/$APP_NAME-$VERSION/install.sh"

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