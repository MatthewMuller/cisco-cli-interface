CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I../include
LIBS = -lpthread -Wl,--gc-sections

# Test executables
TEST_EXEC = test_cisco_commands
FILE_TREE_TESTS = test_file_tree_create test_file_tree_add_child test_file_tree_free \
                  test_file_tree_select test_file_tree_count_selected test_file_tree_delete_selected_recursive \
                  test_file_tree_delete_selected test_file_tree_build_recursive test_file_tree_build \
                  test_file_tree_get_flat_list

# Common dependencies
FRAMEWORK_DEPS = framework/test_framework.o
SRC_DEPS = ../src/file_tree.o ../src/cisco_commands.o

all: $(TEST_EXEC) $(FILE_TREE_TESTS)

# Main test executable
$(TEST_EXEC): $(FRAMEWORK_DEPS) unit/test_cisco_commands.o $(SRC_DEPS)
	$(CC) $^ -o $@ $(LIBS)

# File tree tests (pattern rule)
$(FILE_TREE_TESTS): %: $(FRAMEWORK_DEPS) unit/%.o $(SRC_DEPS)
	$(CC) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_EXEC) $(FILE_TREE_TESTS)
	./$(TEST_EXEC) && for t in $(FILE_TREE_TESTS); do echo "Running $$t..."; ./$$t; done

test-verbose: $(TEST_EXEC) $(FILE_TREE_TESTS)
	./$(TEST_EXEC) --verbose && for t in $(FILE_TREE_TESTS); do echo "Running $$t..."; ./$$t --verbose; done

clean:
	rm -f framework/*.o unit/*.o ../src/*.o $(TEST_EXEC) $(FILE_TREE_TESTS)

help:
	@echo "Targets: all test test-verbose clean help"

.PHONY: all test test-verbose clean help
