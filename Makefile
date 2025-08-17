CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LIBS = -lncurses -lpthread
TARGET = cisco-cli-interface
SOURCES = main.c serial.c ui.c cisco_commands.c file_tree.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

install-deps:
	sudo apt-get update
	sudo apt-get install -y libncurses5-dev libncursesw5-dev 