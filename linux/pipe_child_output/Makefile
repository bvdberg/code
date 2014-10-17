CFLAGS=-Wall -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
all: guard test

guard: guard.c
	@gcc $(CFLAGS) guard.c -o guard

test: test.c
	@gcc test.c -o test -Wall

