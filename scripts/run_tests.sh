#!/bin/bash

# Test runner script for Cisco CLI Interface
# This script builds and runs the unit tests

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're in the right directory
if [ ! -f "Makefile" ]; then
    print_error "Makefile not found. Please run this script from the project root directory."
    exit 1
fi

# Build the test runner
print_status "Building test runner..."
make test

if [ $? -eq 0 ]; then
    print_success "Test runner built successfully!"
else
    print_error "Failed to build test runner!"
    exit 1
fi

# Run the tests
print_status "Running unit tests..."
echo "====================================="

./build/test-runner

# Check test results
if [ $? -eq 0 ]; then
    print_success "All tests passed!"
    exit 0
else
    print_error "Some tests failed!"
    exit 1
fi
