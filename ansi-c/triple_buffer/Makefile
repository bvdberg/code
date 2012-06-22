all: test

test: main.c circbuf.c
	gcc circbuf.c -c -Wall
	gcc main.c circbuf.o -lpthread -Wall -o test

clean:
	rm -f *.o test
