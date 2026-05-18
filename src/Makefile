CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lglfw -lGL -ldl -lm

SRC = $(wildcard core/*.c) /
	  $(wildcard renderer/*.c) /
	  $(wildcard math/*.c)/
	  $(wildcard *.c)
	  
OBJ = $(SRC:.c=.o)

TARGET = demo

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)