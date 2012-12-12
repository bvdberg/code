CFLAGS=-Wall -Wextra -Wno-unused

all: test test2

test: main.cpp
	g++ $(CFLAGS) main.cpp -o test

test2: main.cpp
	g++ $(CFLAGS) main.cpp -o test2 -fno-elide-constructors

clean:
	rm -f *.o test


