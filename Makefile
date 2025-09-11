CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude -MMD -MP
LIBS = -lncurses -lpthread
TARGET = build/cisco-cli-interface
SOURCES = src/main.c src/serial.c src/ui.c src/cisco_commands.c src/file_tree.c
OBJECTS = $(SOURCES:src/%.c=build/%.o)
DEPENDS = $(OBJECTS:.o=.d)

.PHONY: all clean release install-deps help

all: $(TARGET)

release: CFLAGS = -Wall -Wextra -std=c99 -O2 -DNDEBUG -Iinclude -MMD -MP
release: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(DEPENDS)
	rm -rf build

install-deps:
	sudo apt-get update
	sudo apt-get install -y libncurses5-dev libncursesw5-dev build-essential

help:
	@echo "Available targets:"
	@echo "  all         - Build debug version (default)"
	@echo "  release     - Build optimized release version"
	@echo "  clean       - Remove build artifacts"
	@echo "  install-deps- Install system dependencies"
	@echo "  help        - Show this help"

# Include dependency files
-include $(DEPENDS)
