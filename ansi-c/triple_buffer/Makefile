all: test

test: main.c triple_buf.c
	gcc triple_buf.c -c -Wall
	gcc main.c triple_buf.o -lpthread -Wall -o test

clean:
	rm -f *.o test
