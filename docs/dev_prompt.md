# Cisco CLI Interface - Repository Context Prompt

Use this prompt to provide context to LLMs about the Cisco CLI Interface repository before asking questions about the codebase.

---

## Repository Overview

**Cisco CLI Interface** is a C application that provides an interactive ncurses-based interface to manage files and directories on Cisco switches via serial connection. It's designed for network administrators who need to safely browse, select, and delete files from Cisco switch flash memory.

## Core Purpose & Functionality

- **Interactive file management** for Cisco switches via serial connection
- **Flash directory tree browsing** with expandable/collapsible directory structure
- **Batch file deletion** with multi-selection capabilities
- **Binary file protection** with special confirmation for critical files (.bin, .tar, .pkg)
- **Real-time status updates** showing connection and operation status
- **Automatic flash initialization** handling switch boot sequence

## Architecture & Design

### Modular C Architecture
- **main.c**: Application entry point, main loop, and state management
- **serial.c**: Low-level serial communication with termios configuration
- **cisco_commands.c**: High-level Cisco switch command interface and protocol handling
- **file_tree.c**: File system tree management, parsing, and operations
- **ui.c**: ncurses-based terminal user interface with color support
- **cisco_cli.h**: Central header with all data structures and function declarations

### Key Data Structures
- `app_state_t`: Main application state (serial connection, UI state, file tree)
- `serial_conn_t`: Serial connection with mutex-protected communication
- `dir_node_t`: Tree node for file/directory representation with parent/child relationships
- `ui_state_t`: UI state management for different views (main menu, tree view, selection)

### Communication Flow
1. **Serial Connection**: Establishes connection to Cisco switch via serial port
2. **Flash Initialization**: Automatically handles `flash_init` command and boot sequence
3. **Directory Parsing**: Recursively builds file tree from `dir` command output
4. **User Interaction**: ncurses UI for navigation, selection, and operations
5. **Batch Operations**: Executes multiple delete commands with confirmation

## Development Environment

### Docker-First Development
- **Docker Compose setup** with Ubuntu 22.04 base image
- **Serial port access** via `/dev` mounting for hardware interaction
- **Build dependencies**: GCC, ncurses, pthread libraries
- **Development scripts**: `./scripts/dev-setup.sh` for build, test, clean operations

### Build System
- **Makefile-based** with C99 standard and comprehensive warnings
- **Modular compilation** with separate object files
- **Dependency management** via apt packages in Docker environment

### Development Setup Script (`dev-setup.sh`)

The `./scripts/dev-setup.sh` script is the **preferred method** for building binaries locally and managing the development environment. This script provides a Docker-based development workflow that eliminates the need to install dependencies locally.

#### Why Use `dev-setup.sh`?

- **No local dependencies required**: All build tools, libraries, and dependencies are contained within the Docker container
- **Consistent environment**: Ensures identical build environment across different development machines
- **Isolated development**: Prevents conflicts with system-installed packages
- **Easy cleanup**: Simple commands to clean up build artifacts and containers

#### Available Commands

```bash
# Build the development environment (required first step)
./scripts/dev-setup.sh build

# Start an interactive development shell
./scripts/dev-setup.sh shell

# Build the application in container and copy binary to host
./scripts/dev-setup.sh debug

# Run unit tests in container
./scripts/dev-setup.sh test

# Clean up Docker resources (containers, images, volumes)
./scripts/dev-setup.sh clean

# Clean test artifacts only
./scripts/dev-setup.sh clean-tests

# Show help and usage information
./scripts/dev-setup.sh help
```

#### Development Workflow

1. **Initial Setup**: Run `./scripts/dev-setup.sh build` to create the Docker environment
2. **Development**: Use `./scripts/dev-setup.sh shell` for interactive development
3. **Building**: Use `./scripts/dev-setup.sh debug` to build the application with debug symbols
4. **Testing**: Use `./scripts/dev-setup.sh test` to run unit tests
5. **Cleanup**: Use `./scripts/dev-setup.sh clean` when done or to reset environment

#### Important Notes for LLM Development

**Critical**: When making code changes, the LLM must follow this sequence:

1. **Clean first**: Run `./scripts/dev-setup.sh clean` to remove old containers and images
2. **Rebuild environment**: Run `./scripts/dev-setup.sh build` to create fresh container with updated code
3. **Compile**: Run `./scripts/dev-setup.sh debug` to build the application with debug symbols
4. **Test**: Run `./scripts/dev-setup.sh test` to verify changes

This ensures that:
- Code changes are properly reflected in the container
- No stale build artifacts interfere with compilation
- Dependencies are correctly resolved in the fresh environment
- Tests run against the updated codebase

The script automatically handles:
- Docker and Docker Compose availability checks
- Container building with all necessary dependencies
- Binary extraction from container to host system
- Proper file permissions on extracted binaries
- Test execution in isolated environment

### Testing Framework
- **Custom test framework** in `tests/framework/` with assertion macros
- **Mock support** for serial communication functions
- **Unit tests** for core functions like `cisco_wait_for_prompt`
- **Integration tests** for compilation and basic functionality

## Key Technical Details

### Serial Communication
- **Termios configuration** for 8N1 serial communication
- **Timeout handling** with configurable baud rates (9600-115200)
- **Thread-safe operations** using pthread mutexes
- **Buffer management** with 4KB circular buffer

### Cisco Protocol Handling
- **Prompt detection** using pattern matching for ": " endings
- **Command execution** with automatic newline and response waiting
- **Directory parsing** from `dir` command output with file type detection
- **Error handling** with timeout and retry mechanisms

### User Interface
- **ncurses-based** with color support and keyboard navigation
- **Multi-view system**: Main menu, tree view, selection view
- **Real-time updates** with status window and progress indicators
- **Confirmation dialogs** for destructive operations

### File Operations
- **Tree traversal** with recursive directory building
- **File type detection** for binary files (.bin, .tar, .pkg)
- **Selection management** with visual indicators
- **Batch deletion** with individual file confirmation

## Safety & Error Handling

### Critical Safety Features
- **Binary file protection** with extra confirmation for system files
- **Connection validation** before operations
- **Timeout mechanisms** to prevent hanging
- **Error recovery** with graceful degradation

### Error Handling Patterns
- **Return code validation** for all system calls
- **Resource cleanup** in error paths
- **User feedback** for all operations
- **Graceful shutdown** with proper resource deallocation

## Current Branch & Development Status

- **Current branch**: `refactor/cleanup-tech-debt` (clean working tree)
- **Development focus**: Code cleanup and technical debt reduction
- **Testing status**: Unit tests implemented for core functions
- **Documentation**: Comprehensive README and development guides

## Common Use Cases & Workflows

### Typical User Workflow
1. Connect to Cisco switch via serial cable
2. Run application with serial port argument
3. Browse flash directory tree
4. Select files/folders for deletion
5. Confirm and execute batch deletion

### Development Workflow
1. Use Docker environment for consistent builds
2. Edit code on host system
3. Build and test in container
4. Run unit tests for validation
5. Test with actual hardware

## Dependencies & Requirements

### Runtime Dependencies
- **Linux system** with serial port access
- **ncurses library** for terminal UI
- **pthread library** for threading support
- **Serial port permissions** (dialout group membership)

### Development Dependencies
- **GCC compiler** with C99 support
- **Docker & Docker Compose** for development environment
- **Build tools** (make, git)
- **Testing framework** (custom implementation)

---

**Note**: This repository follows a structured branching strategy with conventional commit messages and emphasizes Docker-based development for consistency across team members and CI/CD environments.
