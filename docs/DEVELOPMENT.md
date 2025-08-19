# Development Guide

This guide explains how to set up and work with the Cisco CLI Interface project using different development approaches.

## Development Approaches

### 1. Docker Development Environment (Recommended)

The Docker approach provides the most isolated and consistent development experience.

#### Setup
```bash
# Build the development environment
./scripts/dev-setup.sh build

# Start development shell
./scripts/dev-setup.sh shell
```

#### Workflow
1. **Edit code** in your preferred IDE/editor on the host
2. **Build changes** with `./scripts/dev-setup.sh compile`
3. **Test changes** with `./scripts/dev-setup.sh test`
4. **Clean up** when done with `./scripts/dev-setup.sh clean`

#### Benefits
- ✅ **Isolated environment** - No system dependencies
- ✅ **Consistent builds** - Same environment across all developers
- ✅ **Easy cleanup** - Remove everything with one command
- ✅ **CI/CD ready** - Same environment in GitHub Actions
- ✅ **Cross-platform** - Works on Linux, macOS, Windows

#### Serial Port Access
The Docker setup mounts your host's `/dev` directory, so serial ports are accessible:
```bash
# Inside the container, you can access:
/dev/ttyUSB0
/dev/ttyS0
# etc.
```

### 2. Local Development

For developers who prefer working directly on their system.

#### Setup
```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y build-essential libncurses5-dev libncursesw5-dev

# Build the project
make clean && make
```

#### Workflow
1. **Edit code** directly
2. **Build** with `make`
3. **Test** with `./scripts/test_compile.sh`

#### Benefits
- ✅ **Faster builds** - No container overhead
- ✅ **Direct access** - All system tools available
- ✅ **IDE integration** - Better debugging support

#### Drawbacks
- ❌ **System pollution** - Dependencies installed globally
- ❌ **Environment differences** - May not match CI/CD
- ❌ **Cleanup required** - Manual dependency removal

## Project Structure

```
cisco-cli-interface/
├── src/                    # Source code
│   ├── main.c             # Main application entry point
│   ├── serial.c           # Serial communication
│   ├── ui.c               # ncurses UI
│   ├── cisco_commands.c   # Cisco switch commands
│   └── file_tree.c        # File tree operations
├── include/               # Header files
│   └── cisco_cli.h        # Main header
├── build/                 # Build artifacts (generated)
├── scripts/               # Build and utility scripts
├── docs/                  # Documentation
├── tests/                 # Test files
├── Dockerfile             # Docker development environment
├── docker-compose.yml     # Docker Compose configuration
├── Makefile               # Build system
└── README.md              # Project documentation
```

## Build System

### Makefile Targets

- `make` - Build the application
- `make clean` - Remove build artifacts
- `make install-deps` - Install system dependencies

### Dependencies

The project requires:
- **GCC compiler** - C99 standard
- **ncurses library** - Terminal UI
- **pthread library** - Threading support

## Testing

### Test Scripts
- `./scripts/test_compile.sh` - Basic compilation tests
- `./scripts/dev-setup.sh test` - Run tests in Docker environment

### Adding Tests
1. Create test files in `tests/` directory
2. Add test compilation to `scripts/test_compile.sh`
3. Consider adding unit test framework (e.g., Unity, CUnit)

## Code Style

### C Standards
- Use C99 standard (`-std=c99`)
- Enable all warnings (`-Wall -Wextra`)

### Naming Conventions
- Functions: `snake_case`
- Variables: `snake_case`
- Constants: `UPPER_SNAKE_CASE`
- Files: `snake_case.c`

### Documentation
- Use Doxygen-style comments for functions
- Include parameter descriptions
- Document return values and error conditions

## Debugging

### Local Development
```bash
# Build with debug symbols
make clean && make

# Run with gdb
gdb ./build/cisco-cli-interface
```

### Docker Development
```bash
# Start shell with debug tools
docker-compose run --rm cisco-cli-dev

# Inside container, build and debug
make clean && make
gdb ./build/cisco-cli-interface
```

## CI/CD Integration

### GitHub Actions
The project includes a GitHub Actions workflow in `.github/workflows/ci.yml` that:
1. Builds the application
2. Runs tests
3. Validates build artifacts

### Local CI Testing
```bash
# Test the CI workflow locally
./scripts/dev-setup.sh build
./scripts/dev-setup.sh compile
./scripts/dev-setup.sh test
```

## Troubleshooting

### Common Issues

#### Docker Issues
```bash
# Permission denied on /dev
sudo chmod 666 /dev/ttyUSB0

# Container can't access serial port
# Ensure user is in dialout group on host
sudo usermod -a -G dialout $USER
```

#### Build Issues
```bash
# Missing ncurses
sudo apt-get install libncurses5-dev

# Compiler not found
sudo apt-get install build-essential
```

#### Serial Port Issues
```bash
# Check available ports
ls -la /dev/tty*

# Check permissions
ls -la /dev/ttyUSB0

# Add user to dialout group
sudo usermod -a -G dialout $USER
```

## Best Practices

### Development
1. **Use Docker for development** - Keeps environment clean
2. **Test frequently** - Run tests after each change
3. **Follow coding standards** - Use consistent style
4. **Document changes** - Update README and comments

### Code Quality
1. **Enable all warnings** - Fix all compiler warnings
2. **Use static analysis** - Consider tools like cppcheck
3. **Memory management** - Check for leaks with valgrind
4. **Error handling** - Always check return values

### Version Control
1. **Use feature branches** - Don't commit directly to main
2. **Write clear commit messages** - Use conventional commits
3. **Test before pushing** - Ensure CI passes locally
4. **Update documentation** - Keep README current

## Future Improvements

### Potential Enhancements
1. **Add unit tests** - Implement proper test framework
2. **Static analysis** - Add cppcheck or similar
3. **Code coverage** - Measure test coverage
4. **Performance profiling** - Add profiling tools
5. **Cross-compilation** - Support for different architectures

### Dependency Management
1. **Consider Conan** - For more complex dependency management
2. **Package managers** - Evaluate vcpkg or similar
3. **Version pinning** - Lock dependency versions
