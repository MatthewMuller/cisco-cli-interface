#!/bin/bash

echo "=== Cisco CLI Interface Installation ==="
echo

# Check if running as root
if [ "$EUID" -eq 0 ]; then
    echo "Please do not run this script as root."
    echo "The application should be installed as a regular user."
    exit 1
fi

# Check for required packages
echo "1. Checking dependencies..."
MISSING_DEPS=""

if ! command -v gcc &> /dev/null; then
    MISSING_DEPS="$MISSING_DEPS build-essential"
fi

if ! pkg-config --exists ncurses 2>/dev/null; then
    MISSING_DEPS="$MISSING_DEPS libncurses5-dev"
fi

if [ -n "$MISSING_DEPS" ]; then
    echo "   Missing dependencies: $MISSING_DEPS"
    echo "   Installing dependencies..."
    sudo apt-get update
    sudo apt-get install -y $MISSING_DEPS
    if [ $? -ne 0 ]; then
        echo "   ✗ Failed to install dependencies"
        exit 1
    fi
    echo "   ✓ Dependencies installed"
else
    echo "   ✓ All dependencies are available"
fi

# Compile the application
echo "2. Compiling application..."
make clean > /dev/null 2>&1
if make > /dev/null 2>&1; then
    echo "   ✓ Compilation successful"
else
    echo "   ✗ Compilation failed"
    echo "   Please check the error messages above"
    exit 1
fi

# Set up serial port permissions
echo "3. Setting up serial port permissions..."
if groups $USER | grep -q dialout; then
    echo "   ✓ User already in dialout group"
else
    echo "   Adding user to dialout group..."
    sudo usermod -a -G dialout $USER
    echo "   ✓ User added to dialout group"
    echo "   Note: You may need to log out and back in for changes to take effect"
fi

# Create symlink in /usr/local/bin (optional)
echo "4. Creating system-wide installation..."
read -p "   Install system-wide? (y/N): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    sudo cp cisco-cli-interface /usr/local/bin/
    if [ $? -eq 0 ]; then
        echo "   ✓ Installed to /usr/local/bin/cisco-cli-interface"
        echo "   You can now run 'cisco-cli-interface' from anywhere"
    else
        echo "   ✗ Failed to install system-wide"
    fi
else
    echo "   Skipped system-wide installation"
    echo "   You can run './cisco-cli-interface' from this directory"
fi

echo
echo "=== Installation Complete ==="
echo "✓ Dependencies installed"
echo "✓ Application compiled"
echo "✓ Serial port permissions configured"
echo
echo "Usage:"
echo "  ./cisco-cli-interface <serial_port> [baud_rate]"
echo
echo "Examples:"
echo "  ./cisco-cli-interface /dev/ttyUSB0"
echo "  ./cisco-cli-interface /dev/ttyS0 115200"
echo
echo "For more information, see README.md" 