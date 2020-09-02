CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter -Wshadow -std=c99 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS

SOURCES=main.c
HEADERS=config.h

all: var_a var_b var_c

var_a: $(SOURCES) $(HEADERS)
	gcc -D_VARIANT=1 $(CFLAGS) $(SOURCES) -o var_a

var_b: $(SOURCES) $(HEADERS)
	gcc -D_VARIANT=2 $(CFLAGS) $(SOURCES) -o var_b

var_c: $(SOURCES) $(HEADERS)
	gcc -D_VARIANT=3 $(CFLAGS) $(SOURCES) -o var_c

clean:
	rm -f *.o var_a var_b var_c


