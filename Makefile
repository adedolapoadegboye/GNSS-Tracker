# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O3

# Target executable
TARGET = gnsstracker

# Source files
SRCS = gnsstracker.c src/serial.c src/init.c src/error.c src/process_nmea_data.c src/handle_commands.c

# Object files
OBJS = $(SRCS:.c=.o)

# Log files
LOGS = $(wildcard logs/*.txt)

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET) $(LOGS)

# Phony targets
.PHONY: all clean