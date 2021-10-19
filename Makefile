CC=clang
CFLAGS+=-Wall -Wextra
CFLAGS+=-lsqlite3
RELEASE_FLAGS=-O3
SRC = $(wildcard src/*.c)
OBJ = $(SRC:c=o)
OS=$(shell uname)

ifeq ($(OS), Linux)
	CFLAGS += -I /usr/local/include/linux
	CURSESFLAGS+=-lbsd
endif

ifeq ($(OS), OpenBSD)
	CFLAGS += -I /usr/local/include
endif

jmp: $(OBJ)
	echo $?
	$(CC) $(CFLAGS) $^ -o jmp

release: $(OBJ)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $^ -o jmp

.c.o:
	$(CC) -o $@ -c $< $(CFLAGS)

all: jmp

clean:
	rm -f src/*.o

.PHONY: all