CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
LIBS = -lncurses -lpthread
TARGET = build/cisco-cli-interface
SOURCES = src/main.c src/serial.c src/ui.c src/cisco_commands.c src/file_tree.c
OBJECTS = $(SOURCES:src/%.c=build/%.o)
DEPENDS = $(OBJECTS:.o=.d)

# Debug and Release configurations
DEBUG_CFLAGS = -Wall -Wextra -std=c99 -g -O0 -DDEBUG -Iinclude -MMD -MP
RELEASE_CFLAGS = -Wall -Wextra -std=c99 -O2 -DNDEBUG -Iinclude -MMD -MP

.PHONY: all clean debug release install-deps help

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
	rm -f $(OBJECTS) $(TARGET) $(DEPENDS)
	rm -rf build

install-deps:
	sudo apt-get update
	sudo apt-get install -y libncurses5-dev libncursesw5-dev build-essential

help:
	@echo "Available targets:"
	@echo "  all         - Build debug version (default)"
	@echo "  debug       - Build with debug symbols and no optimization"
	@echo "  release     - Build optimized release version"
	@echo "  clean       - Remove all build artifacts"
	@echo "  install-deps- Install system dependencies"
	@echo "  help        - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make        - Build debug version"
	@echo "  make release- Build release version"
	@echo "  make clean  - Clean build artifacts"

# Include dependency files
-include $(DEPENDS)
