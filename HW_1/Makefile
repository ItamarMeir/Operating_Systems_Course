CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRCS = EX1.c
TARGET = OS

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean