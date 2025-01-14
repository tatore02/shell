TARGET = shl

CC = gcc
CFLAGS = -Wall -Wextra -O2

SRC = main.c shell.c builtin.c
OBJ = $(SRC:.c=.o)

# Regola principale
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
