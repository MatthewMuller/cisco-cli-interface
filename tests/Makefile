CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I../include
LDFLAGS = -lpthread

# Source files
TEST_FRAMEWORK_SRC = framework/test_framework.c
TEST_SRC = unit/test_cisco_commands.c
SOURCE_SRC = ../src/cisco_commands.c

# Object files
TEST_FRAMEWORK_OBJ = $(TEST_FRAMEWORK_SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)
SOURCE_OBJ = $(SOURCE_SRC:.c=.o)

# Test executable
TEST_EXEC = test_cisco_commands

# Default target
all: $(TEST_EXEC)

# Build test executable
$(TEST_EXEC): $(TEST_FRAMEWORK_OBJ) $(TEST_OBJ) $(SOURCE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile test framework
$(TEST_FRAMEWORK_OBJ): $(TEST_FRAMEWORK_SRC) framework/test_framework.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test file
$(TEST_OBJ): $(TEST_SRC) framework/test_framework.h ../include/cisco_cli.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile source file (only the one we need for testing)
$(SOURCE_OBJ): $(SOURCE_SRC) ../include/cisco_cli.h
	$(CC) $(CFLAGS) -c $< -o $@

# Run tests
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Clean
clean:
	rm -f $(TEST_FRAMEWORK_OBJ) $(TEST_OBJ) $(SOURCE_OBJ) $(TEST_EXEC)

.PHONY: all test clean
