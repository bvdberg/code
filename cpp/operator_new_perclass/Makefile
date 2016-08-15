CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter

test: main.cpp Base.cpp Derived.cpp Context.cpp Base.h Derived.h Context.h
	g++ $(CFLAGS) main.cpp Base.cpp Derived.cpp Context.cpp -o test

clean:
	rm -f *.o test


