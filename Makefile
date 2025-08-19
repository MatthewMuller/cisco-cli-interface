CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
LIBS = -lncurses -lpthread
TARGET = build/cisco-cli-interface
SOURCES = src/main.c src/serial.c src/ui.c src/cisco_commands.c src/file_tree.c
OBJECTS = $(SOURCES:src/%.c=build/%.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -rf build

install-deps:
	sudo apt-get update
	sudo apt-get install -y libncurses5-dev libncursesw5-dev 