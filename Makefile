CC=clang
CFLAGS=-Wall -Wextra
SRC = $(wildcard src/*.c)
OBJ = $(SRC:c=o)

mcd: $(OBJ)
	$(CC) $(CFLAGS) $^ -o mcd

.c.o:
	@echo $@ ": " $<
	$(CC) -o $@ -c $< $(CFLAGS)

all: mcd

clean:
	rm -f src/*.o