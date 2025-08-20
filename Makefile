CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
LIBS = -lncurses -lpthread
TARGET = build/cisco-cli-interface
TEST_TARGET = build/test-runner
SOURCES = src/main.c src/serial.c src/ui.c src/cisco_commands.c src/file_tree.c
TEST_SOURCES = src/test_runner.c src/test_framework.c src/cisco_commands_test.c
OBJECTS = $(SOURCES:src/%.c=build/%.o)
TEST_OBJECTS = $(TEST_SOURCES:src/%.c=build/%.o)

.PHONY: all clean test

all: $(TARGET)

test: $(TEST_TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) -o $(TEST_TARGET)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(TARGET) $(TEST_TARGET)
	rm -rf build

install-deps:
	sudo apt-get update
	sudo apt-get install -y libncurses5-dev libncursesw5-dev

run-tests: test
	./$(TEST_TARGET) 