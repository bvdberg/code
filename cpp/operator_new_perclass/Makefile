CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter -O3

test: main.cpp Base.cpp Derived.cpp Context.cpp Foo.cpp Foo.h Base.h Derived.h Context.h
	g++ $(CFLAGS) main.cpp Base.cpp Derived.cpp Foo.cpp Context.cpp -o test

clean:
	rm -f *.o test


