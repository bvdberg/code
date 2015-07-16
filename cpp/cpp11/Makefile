#CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter
CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter -std=c++1y

all: test1 test2 test3 test4 test5 test6

test1: main.cpp
	clang++ $(CFLAGS) main.cpp -o test1

test2: enums.cpp
	clang++ $(CFLAGS) enums.cpp -o test2

test3: init.cpp
	clang++ $(CFLAGS) init.cpp -o test3

test4: constructor.cpp
	clang++ $(CFLAGS) constructor.cpp -o test4

test5: virtual.cpp
	clang++ $(CFLAGS) virtual.cpp -o test5

test6: rvalue.cpp
	clang++ $(CFLAGS) rvalue.cpp -o test6

clean:
	rm -f *.o test1 test2


