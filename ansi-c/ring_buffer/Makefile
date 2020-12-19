CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter -Wshadow
CFLAGS+=-std=c99 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
CFLAGS+=-O2
CFLAGS+=-g

SOURCES=ringbuf.c main.c
HEADERS=ringbuf.h color.h

test: $(SOURCES) $(HEADERS)
	gcc $(CFLAGS) $(SOURCES) -o test

clean:
	rm -f *.o test


