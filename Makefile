CC=clang
CFLAGS+=-Wall -Wextra
CFLAGS+=-lsqlite3 -lbsd
RELEASE_FLAGS=-O3
SRC = $(wildcard src/*.c)
OBJ = $(SRC:c=o)

jmp: $(OBJ)
	$(CC) $(CFLAGS) $^ -o jmp

release: $(OBJ)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $^ -o jmp

.c.o:
	$(CC) -o $@ -c $< $(CFLAGS)

all: jmp

clean:
	rm -f src/*.o

.PHONY: all