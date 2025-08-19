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

## Installation Options

### Option 1: Docker Development Environment (Recommended)

This approach provides an isolated development environment without installing dependencies on your system.

**Prerequisites:**
- Docker and Docker Compose installed

**Setup:**
```bash
# Clone the repository
git clone <repository-url>
cd cisco-cli-interface

# Build the development environment
./scripts/dev-setup.sh build

# Start development shell
./scripts/dev-setup.sh shell
```

**Development Workflow:**
```bash
# Build the application in container
./scripts/dev-setup.sh compile

# Run tests
./scripts/dev-setup.sh test

# Clean up when done
./scripts/dev-setup.sh clean
```

**Benefits:**
- ✅ No system dependencies installed
- ✅ Consistent environment across team members
- ✅ Easy to clean up and recreate
- ✅ Perfect for CI/CD integration

### Option 2: Traditional Installation

Install dependencies directly on your system.

1. **Install dependencies:**
   ```bash
   sudo apt-get update
   sudo apt-get install -y libncurses5-dev libncursesw5-dev build-essential
   ```

2. **Clone and build:**
   ```bash
   git clone <repository-url>
   cd cisco-cli-interface
   make
   ```

3. **Set up serial port permissions:**
   ```bash
   sudo usermod -a -G dialout $USER
   # Log out and back in for changes to take effect
   ```

### Option 3: Automated Installation Script

Use the provided installation script for a guided setup.

```bash
git clone <repository-url>
cd cisco-cli-interface
./scripts/install.sh
```

## Usage

### Basic Usage

```bash
./cisco-cli-interface <serial_port> [baud_rate]
```

### Examples

```bash
# Connect to USB serial adapter at default 9600 baud
./cisco-cli-interface /dev/ttyUSB0

# Connect to built-in serial port at 115200 baud
./cisco-cli-interface /dev/ttyS0 115200

# Connect to USB serial adapter at 19200 baud
./cisco-cli-interface /dev/ttyUSB1 19200
```

### Switch Setup

1. **Connect your computer to the Cisco switch** via serial cable or USB-to-serial adapter
2. **Power on the switch** and interrupt the boot sequence (usually by pressing Ctrl+Break or similar)
3. **Run the application** with the appropriate serial port
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

1. **Backup your switch configuration** before using this tool
2. **Binary files (.bin, .tar, .pkg)** are critical for switch operation - be very careful when deleting these
3. **Test on non-production equipment** first
4. **Ensure proper serial connection** before running deletion operations

## Development

### Building from Source

```bash
make clean
make
```

### Project Structure

- `main.c` - Main application entry point and flow control
- `serial.c` - Serial communication functions
- `cisco_commands.c` - Cisco switch command interface
- `file_tree.c` - File tree management and operations
- `ui.c` - ncurses-based user interface
- `cisco_cli.h` - Header file with all declarations

### Adding Features

The modular design makes it easy to add new features:

1. **New commands** - Add to `cisco_commands.c`
2. **UI improvements** - Modify `ui.c`
3. **File operations** - Extend `file_tree.c`

### Branching Strategy

This project follows a structured branching strategy for organized development:

#### Branch Types
- `feat/description` - A new feature
- `fix/description` - A bug fix
- `docs/description` - Documentation only changes
- `build/description` - Changes that affect the build system or external dependencies
- `ci/description` - Changes to our CI configuration files and scripts
- `perf/description` - A code change that improves performance
- `refactor/description` - A code change that neither fixes a bug nor adds a feature
- `style/description` - Changes that do not affect the meaning of the code (white-space, formatting, missing semi-colons, etc)
- `test/description` - Adding missing tests or correcting existing tests

#### Workflow
1. Create feature branch from `main`
2. Make changes with conventional commit messages
3. Submit pull request to `main`
5. Feature branche merged to `main` for production releases

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## Support

For issues and questions:
1. Check the troubleshooting section
2. Review the code comments
3. Open an issue on the repository
