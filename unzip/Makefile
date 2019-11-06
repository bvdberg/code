CFLAGS=-O2 -Wshadow -Wall -Wextra -Wno-unused -Wno-unused-parameter -std=c99 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I.

OBJS=miniz/miniz_tinfl.c main.c
HEADERS= miniz/miniz_common.h miniz/miniz_tinfl.h

test: $(OBJS) $(HEADERS)
	gcc $(CFLAGS) $(OBJS) -o test

clean:
	rm -f *.o test


