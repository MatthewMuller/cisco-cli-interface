CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
LIBS = -lncurses -lpthread
TARGET = build/cisco-cli-interface
SOURCES = src/main.c src/serial.c src/ui.c src/cisco_commands.c src/file_tree.c
OBJECTS = $(SOURCES:src/%.c=build/%.o)

# Debug and Release configurations
DEBUG_CFLAGS = -Wall -Wextra -std=c99 -g -O0 -DDEBUG -Iinclude
RELEASE_CFLAGS = -Wall -Wextra -std=c99 -O2 -DNDEBUG -Iinclude

.PHONY: all clean debug release

all: debug

debug: CFLAGS = $(DEBUG_CFLAGS)
debug: $(TARGET)

release: CFLAGS = $(RELEASE_CFLAGS)
release: $(TARGET)

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
