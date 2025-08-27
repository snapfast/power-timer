#!/bin/bash

# Build script for Power Timer application

set -e

echo "Building Power Timer..."

# Create build directory
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Configure with meson
meson setup . ..

# Build the application
ninja

echo "Build complete! Run './build/power-timer' to start the application."
echo "To install system-wide, run 'sudo ninja install' from the build directory."