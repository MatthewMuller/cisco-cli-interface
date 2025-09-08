#!/bin/bash

# Development setup script for Cisco CLI Interface
# This script provides easy commands for working with Docker development environment

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

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if Docker is installed
check_docker() {
    if ! command -v docker &> /dev/null; then
        print_error "Docker is not installed. Please install Docker first."
        print_status "Visit: https://docs.docker.com/get-docker/"
        exit 1
    fi

    # Check for Docker Compose V2 (preferred) or V1
    if docker compose version &> /dev/null; then
        DOCKER_COMPOSE_CMD="docker compose"
    elif command -v docker-compose &> /dev/null; then
        DOCKER_COMPOSE_CMD="docker-compose"
    else
        print_error "Docker Compose is not installed. Please install Docker Compose first."
        print_status "Visit: https://docs.docker.com/compose/install/"
        exit 1
    fi
}

# Build the development environment
build_dev() {
    print_status "Building development environment..."
    $DOCKER_COMPOSE_CMD build
    print_success "Development environment built successfully!"
}

# Start development shell
dev_shell() {
    print_status "Starting development shell..."
    print_warning "Note: Serial devices will be mounted from host system"
    $DOCKER_COMPOSE_CMD run --rm cisco-cli-dev
}

# Build the application in container
build_app() {
    print_status "Building application in container..."
    docker run --rm cisco-cli-interface-cisco-cli-dev:latest bash -c "cd /app && make clean && make"

    if [ $? -eq 0 ]; then
        print_status "Copying binary to host build directory..."
        mkdir -p build
        docker run --rm cisco-cli-interface-cisco-cli-dev:latest cat /app/build/cisco-cli-interface > build/cisco-cli-interface
        chmod +x build/cisco-cli-interface
        print_success "Application built successfully and copied to ./build/cisco-cli-interface!"
    else
        print_error "Build failed!"
        exit 1
    fi
}

# Build debug version of the application in container
build_debug() {
    print_status "Building debug version of application in container..."
    docker run --rm cisco-cli-interface-cisco-cli-dev:latest bash -c "cd /app && make clean && make debug"

    if [ $? -eq 0 ]; then
        print_status "Copying debug binary to host build directory..."
        mkdir -p build
        docker run --rm cisco-cli-interface-cisco-cli-dev:latest cat /app/build/cisco-cli-interface > build/cisco-cli-interface
        chmod +x build/cisco-cli-interface
        print_success "Debug application built successfully and copied to ./build/cisco-cli-interface!"
        print_status "Binary includes debug symbols and is ready for debugging in Cursor IDE"
    else
        print_error "Debug build failed!"
        exit 1
    fi
}

# Run tests in container
run_tests() {
    print_status "Running unit tests in container..."
    $DOCKER_COMPOSE_CMD run --rm cisco-cli-dev bash -c "cd /app/tests && make clean && make && make test"
}

# Clean up containers and images
cleanup() {
    print_status "Cleaning up Docker resources..."
    $DOCKER_COMPOSE_CMD down --rmi all --volumes --remove-orphans
    print_success "Cleanup completed!"
}

# Clean test artifacts
clean_tests() {
    print_status "Cleaning test artifacts..."
    $DOCKER_COMPOSE_CMD run --rm cisco-cli-dev bash -c "cd /app/tests && make clean"
    print_success "Test cleanup completed!"
}

# Show help
show_help() {
    echo "Cisco CLI Interface - Development Setup"
    echo
    echo "Usage: $0 [COMMAND]"
    echo
    echo "Commands:"
    echo "  build       Build the development environment"
    echo "  shell       Start an interactive development shell"
    echo "  compile     Build the application in container"
    echo "  debug       Build debug version with symbols for IDE debugging"
    echo "  test        Run unit tests in container"
    echo "  clean       Clean up Docker resources"
    echo "  clean-tests Clean test artifacts"
    echo "  help        Show this help message"
    echo
    echo "Examples:"
    echo "  $0 build      # Build the Docker environment"
    echo "  $0 shell      # Start development shell"
    echo "  $0 compile    # Build the application"
    echo "  $0 debug      # Build debug version for IDE debugging"
    echo "  $0 test       # Run unit tests"
    echo "  $0 clean-tests # Clean test artifacts"
    echo
    echo "Development Workflow:"
    echo "1. Run '$0 build' to set up the environment"
    echo "2. Run '$0 debug' to build debug version"
    echo "3. Use Cursor IDE debugger with the built binary"
    echo "4. Edit code in your IDE (changes are synced)"
    echo "5. Run '$0 debug' again to rebuild with changes"
    echo "6. Test your application"
}

# Main script logic
main() {
    check_docker

    case "${1:-help}" in
        build)
            build_dev
            ;;
        shell)
            dev_shell
            ;;
        compile)
            build_app
            ;;
        debug)
            build_debug
            ;;
        test)
            run_tests
            ;;
        clean)
            cleanup
            ;;
        clean-tests)
            clean_tests
            ;;
        help|--help|-h)
            show_help
            ;;
        *)
            print_error "Unknown command: $1"
            show_help
            exit 1
            ;;
    esac
}

# Run main function with all arguments
main "$@"
