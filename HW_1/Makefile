# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11

# Source files
SRCS = EX1.c

# Output executable
TARGET = OS

# Default target
all:    $(TARGET)

# Rule to build the target
$(TARGET):  $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean up build files
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean