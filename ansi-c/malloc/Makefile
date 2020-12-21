CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter -Wshadow -std=c99 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
CFLAGS+=-I . -O2

SOURCES=utils/list.c blk.c main.c
HEADERS=utils/list.h blk.h

test: $(SOURCES) $(HEADERS)
	gcc $(CFLAGS) $(SOURCES) -o test

clean:
	rm -f *.o test


