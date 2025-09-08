CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I../include
LIBS = -lpthread -Wl,--gc-sections

# Test executables
TEST_EXEC = test_cisco_commands
FILE_TREE_TEST_EXEC = test_file_tree

# Default target
all: $(TEST_EXEC) $(FILE_TREE_TEST_EXEC)

# Build test executables
$(TEST_EXEC): framework/test_framework.o unit/test_cisco_commands.o ../src/cisco_commands.o
	$(CC) $^ -o $@ $(LIBS)

$(FILE_TREE_TEST_EXEC): framework/test_framework.o unit/test_file_tree.o ../src/file_tree.o ../src/cisco_commands.o
	$(CC) $^ -o $@ $(LIBS)

# Pattern rules for compilation
%.o: %.c
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
	rm -f framework/*.o unit/*.o ../src/*.o $(TEST_EXEC) $(FILE_TREE_TEST_EXEC)

# Help
help:
	@echo "Available targets:"
	@echo "  all         - Build all test executables"
	@echo "  test        - Run all tests"
	@echo "  test-verbose- Run all tests with verbose output"
	@echo "  clean       - Clean test files"
	@echo "  help        - Show this help"

.PHONY: all test test-verbose clean help
