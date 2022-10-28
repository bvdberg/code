CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter -Wshadow -std=c99 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -O2
CFLAGS+=-L/usr/lib32

SOURCES=data.c main.c
HEADERS=data.h

all: test32 test64

test32: $(SOURCES) $(HEADERS)
	gcc -m32 $(CFLAGS) $(SOURCES) -o test32

test64: $(SOURCES) $(HEADERS)
	gcc $(CFLAGS) $(SOURCES) -o test64

clean:
	rm -f *.o test32 test64


