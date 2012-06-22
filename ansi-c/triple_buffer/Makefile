all: test

test: main.c triple_buf.c
	gcc -O2 triple_buf.c -c -Wall
	gcc -O2 main.c triple_buf.o -lpthread -Wall -o test

clean:
	rm -f *.o test
