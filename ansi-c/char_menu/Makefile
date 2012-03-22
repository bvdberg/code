CFLAGS=-Wall -Wextra

all: test debug

debug: main.c
	gcc $(CFLAGS) -D DEBUG main.c -o debug

test: main.c
	gcc $(CFLAGS) main.c -o test

clean:
	rm -f *.o test


