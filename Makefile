CC=clang
CFLAGS+=-Wall -Wextra
CFLAGS+=-l sqlite3
RELEASE_FLAGS=-O3
SRC = $(wildcard src/*.c)
OBJ = $(SRC:c=o)

mcd: $(OBJ)
	$(CC) $(CFLAGS) $^ -o mcd

release: $(OBJ)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $^ -o mcd

.c.o:
	$(CC) -o $@ -c $< $(CFLAGS)

all: mcd

clean:
	rm -f src/*.o

.PHONY: all