CC=clang
CFLAGS=-Wall -Wextra

all: *.o
	$(CC) $(CFLAGS) *.o -o mcd

test: *.o
	$(CC) $(CFLAGS) *.o -o mcd_test
	@./mcd_test

*.o: *.c
	$(CC) $(CFLAGS) -c $?

clean:
	rm -f *.o mcd_test