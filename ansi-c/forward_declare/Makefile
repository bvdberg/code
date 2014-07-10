# NOTE -std=c11 is needed if using multiple headers with same typedef struct _b b;
CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter

test: main.c
	gcc $(CFLAGS) main.c -o test

clean:
	rm -f *.o test


