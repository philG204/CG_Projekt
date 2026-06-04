CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lglfw -lGLEW -lGL -ldl -lm

SRC = $(wildcard src/*.c) \
	  $(wildcard src/core/*.c) \
	  $(wildcard src/renderer/*.c) \
	  $(wildcard src/math/*.c)
	  
OBJ = $(SRC:.c=.o)

TARGET = demo

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)