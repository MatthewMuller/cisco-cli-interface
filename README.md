# Cisco CLI Interface

A C application that provides an interactive interface to manage files and directories on Cisco switches via serial connection.

## Features

- **Interactive ncurses-based UI** - Clean, navigable interface
- **Flash directory tree view** - Browse the complete flash:/ filesystem structure
- **File/folder selection** - Select multiple items for deletion
- **Batch deletion** - Delete multiple selected files and folders at once
- **Binary file protection** - Special confirmation for .bin, .tar, and .pkg files
- **Real-time status updates** - See connection and operation status
- **Automatic flash initialization** - Handles the switch boot sequence automatically

## Requirements

- Linux system
- GCC compiler
- ncurses development library
- Serial port access (USB-to-serial adapter or built-in serial port)

## Quick Start (Docker - Recommended)

The easiest way to get started is using the provided Docker development environment:

```bash
# Clone the repository
git clone <repository-url>
cd cisco-cli-interface

# Build the development environment
./scripts/docker-dev.sh build

# Build the application
./scripts/docker-dev.sh debug

# Run the application
./build/cisco-cli-interface /dev/ttyUSB0
```

## Development Environment

### Docker Development Setup

The project includes a complete Docker-based development environment that handles all dependencies automatically.

**Available Commands:**
```bash
./scripts/docker-dev.sh build      # Build the Docker environment
./scripts/docker-dev.sh shell      # Start interactive development shell
./scripts/docker-dev.sh debug      # Build debug version (recommended for development)
./scripts/docker-dev.sh compile    # Build release version
./scripts/docker-dev.sh test       # Run unit tests
./scripts/docker-dev.sh clean      # Clean up Docker resources
```

**Development Workflow:**
1. `./scripts/docker-dev.sh build` - Set up the environment (first time only)
2. `./scripts/docker-dev.sh debug` - Build debug version with symbols
3. Edit code in your IDE (changes are synced automatically)
4. `./scripts/docker-dev.sh debug` - Rebuild with changes
5. Test your application

The debug build includes symbols for IDE debugging and outputs to `./build/cisco-cli-interface`.

### Manual Installation

If you prefer to install dependencies directly on your system:

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y libncurses5-dev libncursesw5-dev build-essential

# Clone and build
git clone <repository-url>
cd cisco-cli-interface
make debug

# Set up serial port permissions
sudo usermod -a -G dialout $USER
# Log out and back in for changes to take effect
```

## Usage

### Basic Usage

```bash
./build/cisco-cli-interface <serial_port> [baud_rate]
```

### Examples

```bash
# Connect to USB serial adapter at default 9600 baud
./build/cisco-cli-interface /dev/ttyUSB0

# Connect to built-in serial port at 115200 baud
./build/cisco-cli-interface /dev/ttyS0 115200

# Connect to USB serial adapter at 19200 baud
./build/cisco-cli-interface /dev/ttyUSB1 19200
```

### Switch Setup

1. **Connect your computer to the Cisco switch** via serial cable or USB-to-serial adapter
2. **Run the application** with the appropriate serial port
3. **Power on the switch** and interrupt the boot sequence (usually by pressing Mode button)
4. **The application will automatically:**
   - Wait for the switch to show the flash_init prompt
   - Send the flash_init command
   - Wait for the switch to be ready for commands

### Interface Navigation

#### Main Menu
- **Arrow keys** - Navigate menu options
- **Enter** - Select option
- **q** - Quit application

#### Tree View
- **Arrow keys** - Navigate through files/folders
- **Space** - Expand/collapse directories
- **r** - Refresh file tree
- **q** - Return to main menu

#### Selection View
- **Arrow keys** - Navigate through files/folders
- **Space** - Select/deselect items
- **d** - Delete selected items
- **r** - Refresh file tree
- **q** - Return to main menu

## File Types and Protection

The application recognizes different file types and provides appropriate handling:

- **Regular files** - Standard deletion with confirmation
- **Directories** - Deleted only when empty (children deleted first)
- **Binary files** (.bin, .tar, .pkg) - Extra confirmation required due to importance

## Troubleshooting

### Serial Port Issues

1. **Check if device exists:**
   ```bash
   ls -la /dev/ttyUSB*
   ls -la /dev/ttyS*
   ```

2. **Check permissions:**
   ```bash
   ls -la /dev/ttyUSB0
   ```

3. **Add user to dialout group:**
   ```bash
   sudo usermod -a -G dialout $USER
   ```

### Connection Issues

- **Wrong baud rate** - Try different baud rates (9600, 19200, 38400, 57600, 115200)
- **Wrong serial port** - Check device manager or `dmesg` for USB device detection
- **Switch not ready** - Ensure switch is in boot mode and showing flash_init prompt

### Compilation Issues

- **Missing ncurses** - Install with `sudo apt-get install libncurses5-dev`
- **Permission denied** - Ensure you have write permissions in the directory

## Safety Notes

⚠️ **Important Safety Warnings:**

1. **Backup your switch configuration** before using this tool if you want to preserve files on switch
2. **Binary files (.bin, .tar, .pkg)** are critical for switch operation - be very careful when deleting these
3. **Test on non-production equipment** first
4. **Ensure proper serial connection** before running deletion operations

## Development

### Project Structure

- `src/main.c` - Main application entry point and flow control
- `src/serial.c` - Serial communication functions
- `src/cisco_commands.c` - Cisco switch command interface
- `src/file_tree.c` - File tree management and operations
- `src/ui.c` - ncurses-based user interface
- `include/cisco_cli.h` - Header file with all declarations

### Building

The project uses a Makefile with debug and release configurations:

```bash
make debug    # Build with debug symbols (default)
make release  # Build optimized release version
make clean    # Remove build artifacts
```

### Testing

Run the test suite using the development environment:

```bash
./scripts/docker-dev.sh test
```



## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing to Repo

This section talks about how you can contribute to the project!

### Branching Strategy

This project follows a trunk-based development approach:

- **`main`** - Production-ready code
- **`develop`** - Integration branch for CI/CD testing (no PRs required)
- **Feature branches** - Branched directly from `main` using conventional naming:
  - `feat/description` - New features
  - `fix/description` - Bug fixes
  - `refactor/description` - Code refactoring
  - `docs/description` - Documentation changes
  - `test/description` - Test improvements

### Workflow
1. Create feature branch from `main`
2. Make changes with conventional commit messages
3. Submit pull request to `main`
4. Use `develop` branch for CI/CD testing without PRs

### How to Contribute

1. Fork the repository
2. Create a feature branch from `main`
3. Make your changes
4. Test thoroughly
5. Submit a pull request to `main`

## Support

For issues and questions:
1. Check the troubleshooting section
2. Review the code comments
3. Open an issue on the repository
