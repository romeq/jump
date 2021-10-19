CC=clang
CFLAGS+=-Wall -Wextra -Wpedantic
CFLAGS+=-lsqlite3
RELEASE_FLAGS=-O3
SRC = $(wildcard src/*.c)
OBJ = $(SRC:c=o)
OS = $(shell uname)

ifeq ($(OS), OpenBSD)
	CFLAGS+=-lbsd
endif
ifeq ($(OS), Linux)
	CFLAGS+=-lbsd
endif

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