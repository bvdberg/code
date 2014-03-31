CFLAGS=-Wall -Wextra -Wno-unused -O2
#-std=c99

all: server client

server: server.c
	@echo "building server"
	@gcc $(CFLAGS) server.c -o server -lrt

client: client.c
	@echo "building client"
	@gcc $(CFLAGS) client.c -o client -lrt

clean:
	rm -f *.o server client


