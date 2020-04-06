CFLAGS=-O2 -Wall -Wextra -Wno-unused -Wno-unused-parameter -Wshadow -std=c99 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS

HEADERS = nmea_parser.h
SOURCES = nmea_parser.c main.c

test: $(HEADERS) $(SOURCES)
	gcc $(CFLAGS) $(SOURCES) -o test

clean:
	rm -f *.o test


