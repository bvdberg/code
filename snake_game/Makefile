CFLAGS=-Wall -Wextra -g -std=c11 -Wno-unused-parameter -Wno-unused-function -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS

test: main.c
	@clang $(CFLAGS) -O2 main.c -o test

clean:
	rm -f *.o test


