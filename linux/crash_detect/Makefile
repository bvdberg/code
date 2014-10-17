CFLAGS=-Wall -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
all: guard test-normal test-crash

guard: guard.c
	@gcc $(CFLAGS) guard.c -o guard

test-normal: main.c
	@gcc main.c -o test-normal -Wall

test-crash: main.c
	@gcc main.c -o test-crash -DCRASH -Wall
