CFLAGS=-Wall -Wextra -Wno-unused -O3 -fno-rtti

all: test test2 test3

test: main.cpp
	g++ $(CFLAGS) main.cpp -o test

test2: main2.cpp
	g++ $(CFLAGS) main2.cpp -o test2

test3: main3.cpp
	g++ $(CFLAGS) main3.cpp -o test3

clean:
	rm -f *.o test test2 test3


