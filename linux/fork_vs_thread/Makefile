CC_ARGS=-Wall -Wextra -std=c99 -O2

all: test

test: main.c
		gcc main.c -o test $(CC_ARGS) -lpthread

clean:
	rm -f test
