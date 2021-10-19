CC=clang
CFLAGS+=-Wall -Wextra -lbsd
CFLAGS+=-l sqlite3
RELEASE_FLAGS=-O3
SRC = $(wildcard src/*.c)
OBJ = $(SRC:c=o)

jump: $(OBJ)
	$(CC) $(CFLAGS) $^ -o jmp

release: $(OBJ)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $^ -o jmp

.c.o:
	$(CC) -o $@ -c $< $(CFLAGS)

all: jump

clean:
	rm -f src/*.o

.PHONY: all