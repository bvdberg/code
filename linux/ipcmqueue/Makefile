all: queue.o client server
	
queue.o: queue.cpp queue.h
	g++ -Wall -c queue.cpp -o queue.o

client: client.cpp queue.o
	g++ -Wall client.cpp queue.o -o client

server: server.cpp queue.o
	g++ -Wall server.cpp queue.o -o server

clean:
	rm -f *.o client server

remake:
	clean all

