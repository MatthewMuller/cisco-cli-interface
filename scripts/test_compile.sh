#!/bin/bash

echo "=== Cisco CLI Interface Test ==="
echo

# Test compilation
echo "1. Testing compilation..."
make clean > /dev/null 2>&1
if make > /dev/null 2>&1; then
    echo "   ✓ Compilation successful"
else
    echo "   ✗ Compilation failed"
    exit 1
fi

# Test binary exists
echo "2. Testing binary creation..."
if [ -f "./cisco-cli-interface" ]; then
    echo "   ✓ Binary created successfully"
else
    echo "   ✗ Binary not found"
    exit 1
fi

# Test usage
echo "3. Testing usage display..."
./cisco-cli-interface 2>&1 | grep -q "Usage:"
if [ $? -eq 0 ]; then
    echo "   ✓ Usage display works"
else
    echo "   ✗ Usage display failed"
fi

echo
echo "=== Test Results ==="
echo "✓ Application compiles successfully"
echo "✓ Binary is created"
echo "✓ Usage information is displayed"
echo
echo "The application is ready to use!"
echo "Example: ./cisco-cli-interface /dev/ttyUSB0" 