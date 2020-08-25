CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter -Wshadow -std=c99 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS

SOURCES=main.c
HEADERS=config.h config_a.h config_b.h config_c.h

test: $(SOURCES) $(HEADERS)
	gcc $(CFLAGS) $(SOURCES) -o test

clean:
	rm -f *.o test


