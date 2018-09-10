CFLAGS=-O2 -Wall -Wextra -Wno-unused -Wno-unused-parameter -std=c99 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS

all: test zpipe

test: main.c
	gcc $(CFLAGS) main.c -o test -lz

zpipe: zpipe.c
	gcc $(CFLAGS) zpipe.c -o zpipe -lz

clean:
	rm -f *.o test zpipe


