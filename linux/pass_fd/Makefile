CFLAGS=-O2 -Wall -Wextra -Wno-unused -Wno-unused-parameter -Wshadow -std=c99 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS

all: client server

client: client.c
	gcc $(CFLAGS) client.c -o client

server: server.c
	gcc $(CFLAGS) server.c -o server

clean:
	rm -f *.o client server


