CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter

test: main.cpp Foo.cpp Foo.h Bar.cpp Bar.h
	g++ $(CFLAGS) Foo.cpp Bar.cpp main.cpp -o test

clean:
	rm -f *.o test


