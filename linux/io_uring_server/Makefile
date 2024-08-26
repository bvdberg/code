CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter -Wshadow -std=c99 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
CFLAGS+=-g -O2
#CFLAGS+=-g -O0

SOURCES=logger.c linked_list.c main.c
HEADERS=logger.h linked_list.h

test: $(SOURCES) $(HEADERS)
	gcc $(CFLAGS) $(SOURCES) -luring -o test

clean:
	rm -f *.o test


