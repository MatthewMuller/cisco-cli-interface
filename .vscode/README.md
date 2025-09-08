# Debug Environment Setup for Cisco CLI Interface

This directory contains VS Code/Cursor IDE configuration files for debugging the Cisco CLI Interface project.

## Quick Start

1. **Build the debug environment:**
   ```bash
   ./scripts/dev-setup.sh build
   ```

2. **Build debug version:**
   ```bash
   ./scripts/dev-setup.sh debug
   ```

3. **Start debugging in Cursor IDE:**
   - Press `F5` or go to Run and Debug panel
   - Select "Debug Cisco CLI (Local)" configuration
   - Set breakpoints in your code
   - Start debugging!

## Debug Configurations

### 1. Debug Cisco CLI (Local)
- **Purpose**: Debug the application built locally on your host system
- **Use case**: When you want to debug without Docker overhead
- **Requirements**: Local build tools and dependencies installed

### 2. Debug Cisco CLI (Docker)
- **Purpose**: Debug the application built inside Docker container
- **Use case**: When you want consistent environment across different systems
- **Requirements**: Docker and the debug build from dev-setup.sh

### 3. Attach to Cisco CLI Process
- **Purpose**: Attach debugger to a running process
- **Use case**: When you need to debug a process that's already running
- **Requirements**: Process must be running with debug symbols

## Build Tasks

The following tasks are available in the Command Palette (`Ctrl+Shift+P`):

- **build-debug**: Build with debug symbols (default build task)
- **build-release**: Build optimized release version
- **docker-build-debug**: Build debug version in Docker
- **clean**: Clean build artifacts
- **run-tests**: Run unit tests in Docker
- **docker-shell**: Start interactive Docker development shell

## Debug Features

- **Breakpoints**: Set breakpoints by clicking in the gutter or pressing `F9`
- **Step debugging**: Use `F10` (step over), `F11` (step into), `Shift+F11` (step out)
- **Variable inspection**: Hover over variables or use the Variables panel
- **Call stack**: View function call hierarchy in the Call Stack panel
- **Watch expressions**: Add expressions to monitor in the Watch panel
- **Debug console**: Execute expressions in the current context

## Troubleshooting

### Common Issues

1. **"Program not found" error:**
   - Make sure you've run `./scripts/dev-setup.sh debug` first
   - Check that the binary exists at `./build/cisco-cli-interface`

2. **"GDB not found" error:**
   - Install gdb: `sudo apt-get install gdb`
   - For Docker debugging, gdb is included in the container

3. **Breakpoints not working:**
   - Ensure you're using the debug build (with `-g` flag)
   - Check that source files match the compiled version

4. **Serial device access issues:**
   - For local debugging, ensure your user is in the `dialout` group:
     ```bash
     sudo usermod -a -G dialout $USER
     ```
   - Log out and back in for group changes to take effect

### Debug Symbols

The debug build includes:
- Full debug symbols (`-g`)
- No optimization (`-O0`)
- Debug macros enabled (`-DDEBUG`)
- All warnings enabled (`-Wall -Wextra`)

## Development Workflow

1. **Initial setup:**
   ```bash
   ./scripts/dev-setup.sh build
   ./scripts/dev-setup.sh debug
   ```

2. **Development cycle:**
   - Edit code in Cursor IDE
   - Set breakpoints where needed
   - Press `F5` to start debugging
   - Use debug controls to step through code
   - Make changes and rebuild with `./scripts/dev-setup.sh debug`

3. **Testing:**
   ```bash
   ./scripts/dev-setup.sh test
   ```

## Environment Variables

The debug environment sets these variables:
- `TERM=xterm-256color`: For proper terminal colors
- `DEBUG=1`: Debug mode enabled
- `CC=gcc`: Compiler selection
- `CFLAGS`: Debug-specific compiler flags

## File Structure

```
.vscode/
├── launch.json      # Debug configurations
├── tasks.json       # Build and run tasks
├── settings.json    # Project-specific settings
└── README.md        # This file
```

## Additional Resources

- [VS Code C++ Debugging Guide](https://code.visualstudio.com/docs/cpp/cpp-debug)
- [GDB Documentation](https://www.gnu.org/software/gdb/documentation/)
- [Cisco CLI Interface Documentation](../docs/)
