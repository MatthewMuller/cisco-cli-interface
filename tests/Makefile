CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I../include
LIBS = -lpthread -Wl,--gc-sections
TEST_EXEC = test_cisco_commands
FILE_TREE_TEST_EXEC = test_file_tree

all: $(TEST_EXEC) $(FILE_TREE_TEST_EXEC)

$(TEST_EXEC): framework/test_framework.o unit/test_cisco_commands.o ../src/cisco_commands.o
	$(CC) $^ -o $@ $(LIBS)

$(FILE_TREE_TEST_EXEC): framework/test_framework.o unit/test_file_tree.o ../src/file_tree.o ../src/cisco_commands.o
	$(CC) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_EXEC) $(FILE_TREE_TEST_EXEC)
	./$(TEST_EXEC) && ./$(FILE_TREE_TEST_EXEC)

test-verbose: $(TEST_EXEC) $(FILE_TREE_TEST_EXEC)
	./$(TEST_EXEC) --verbose && ./$(FILE_TREE_TEST_EXEC) --verbose

clean:
	rm -f framework/*.o unit/*.o ../src/*.o $(TEST_EXEC) $(FILE_TREE_TEST_EXEC)

help:
	@echo "Available targets:"
	@echo "  all          - Build all test executables"
	@echo "  test         - Run all tests"
	@echo "  test-verbose - Run tests with verbose output"
	@echo "  clean        - Remove test artifacts"
	@echo "  help         - Show this help"

.PHONY: all test test-verbose clean help
