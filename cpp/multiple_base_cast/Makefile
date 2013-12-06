CFLAGS=-Wall -Wextra -Wno-unused

test: main.cpp
	g++ $(CFLAGS) main.cpp -o test -fdump-class-hierarchy

test2: main.cpp
	clang -cc1 -fdump-record-layouts main.cpp -o test

clean:
	rm -f *.o test *.class


