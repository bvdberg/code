ARGS=-Wall

all: client server

client: rdate-client.c
	gcc rdate-client.c $(ARGS) -o client

server: rdate-server.c
	gcc rdate-server.c $(ARGS) -o server

clean:
	rm -rf client server
