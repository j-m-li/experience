CC = gcc
CFLAGS = -std=c90 -Wall -Iinclude

SRC = src/main.c \
      src/wasm_interpreter.c \
      src/wasm_parser.c \
      src/wasm_sections.c \
      src/wasm_memory.c \
      src/wasm_globals.c \
      src/util.c

OBJ = $(SRC:.c=.o)

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

clean:
	rm -f src/*.o main

.PHONY: all clean