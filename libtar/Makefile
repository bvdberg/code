CFLAGS=-O2 -Wall -Wextra -Wno-unused -Wno-unused-parameter -std=c99 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS

all: mytar mytar.static

mytar: libtar.c
	gcc $(CFLAGS) libtar.c -o mytar -ltar

mytar.static: libtar.c
	gcc $(CFLAGS) libtar.c /usr/lib/libtar.a -o mytar.static

clean:
	rm -f *.o mytar

