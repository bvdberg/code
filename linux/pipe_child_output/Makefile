CFLAGS=-Wall -Wextra -Wno-unused-parameter -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS

all: test reader writer

test: main.c
	@gcc $(CFLAGS) main.c -o test

reader: reader.c
	@gcc reader.c -o reader -Wall

writer: writer.c
	@gcc writer.c -o writer -Wall
