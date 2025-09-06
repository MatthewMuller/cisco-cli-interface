CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I../include
LDFLAGS = -lpthread -Wl,--gc-sections

# Source files for the streamlined framework
TEST_FRAMEWORK_SRC = framework/test_framework.c
TEST_SRC = unit/test_cisco_commands.c unit/test_file_tree.c
SOURCE_SRC = ../src/cisco_commands.c ../src/file_tree.c

# Object files
TEST_FRAMEWORK_OBJ = $(TEST_FRAMEWORK_SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)
SOURCE_OBJ = $(SOURCE_SRC:.c=.o)

# Test executables
TEST_EXEC = test_cisco_commands
FILE_TREE_TEST_EXEC = test_file_tree

# Default target
all: $(TEST_EXEC) $(FILE_TREE_TEST_EXEC)

# Build test executables
$(TEST_EXEC): $(TEST_FRAMEWORK_OBJ) unit/test_cisco_commands.o ../src/cisco_commands.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(FILE_TREE_TEST_EXEC): $(TEST_FRAMEWORK_OBJ) unit/test_file_tree.o ../src/file_tree.o ../src/cisco_commands.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile test framework
$(TEST_FRAMEWORK_OBJ): $(TEST_FRAMEWORK_SRC) framework/test_framework.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test files
unit/test_cisco_commands.o: unit/test_cisco_commands.c framework/test_framework.h ../include/cisco_cli.h
	$(CC) $(CFLAGS) -c $< -o $@

unit/test_file_tree.o: unit/test_file_tree.c framework/test_framework.h ../include/cisco_cli.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile source files
../src/cisco_commands.o: ../src/cisco_commands.c ../include/cisco_cli.h
	$(CC) $(CFLAGS) -c $< -o $@

../src/file_tree.o: ../src/file_tree.c ../include/cisco_cli.h
	$(CC) $(CFLAGS) -c $< -o $@

# Run tests
test: $(TEST_EXEC) $(FILE_TREE_TEST_EXEC)
	@echo "Running cisco_commands tests..."
	./$(TEST_EXEC)
	@echo "Running file_tree tests..."
	./$(FILE_TREE_TEST_EXEC)

# Run tests with verbose output
test-verbose: $(TEST_EXEC) $(FILE_TREE_TEST_EXEC)
	@echo "Running cisco_commands tests with verbose output..."
	./$(TEST_EXEC) --verbose
	@echo "Running file_tree tests with verbose output..."
	./$(FILE_TREE_TEST_EXEC) --verbose

# Clean
clean:
	rm -f $(TEST_FRAMEWORK_OBJ) unit/*.o ../src/*.o $(TEST_EXEC) $(FILE_TREE_TEST_EXEC)

# Help
help:
	@echo "Available targets:"
	@echo "  all         - Build all test executables"
	@echo "  test        - Run all tests"
	@echo "  test-verbose- Run all tests with verbose output"
	@echo "  clean       - Clean test files"
	@echo "  help        - Show this help"

.PHONY: all test test-verbose clean help
