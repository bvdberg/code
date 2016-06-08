CFLAGS=-D_BSD_SOURCE -Wall -Wextra -Wno-unused -Wno-unused-parameter -std=c99

test: main.c
	gcc $(CFLAGS) main.c -o test -pthread

clean:
	rm -f *.o test


