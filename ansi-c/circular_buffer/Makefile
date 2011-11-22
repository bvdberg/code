all: test tester

test: main.c circbuf.c
	gcc circbuf.c -c -Wall
	gcc main.c circbuf.o -lpthread -Wall -o test

tester: tester.cpp circbuf.c
	gcc circbuf.c -c -Wall
	g++ myassert.cpp -c -Wall
	g++ tester.cpp circbuf.o myassert.o -lpthread -Wall -o tester

clean:
	rm -f *.o test tester
