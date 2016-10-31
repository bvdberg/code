CFLAGS1=-Wall -Wextra -Wno-unused -Wno-unused-parameter -std=c99 -O1
CFLAGS2=-Wall -Wextra -Wno-unused -Wno-unused-parameter -std=c99 -O1 -g -fsanitize=address -fno-omit-frame-pointer

all: test1 test2

test1: main.c
	clang-3.5 $(CFLAGS1) main.c -o test1

test2: main.c
	clang-3.5 $(CFLAGS2) main.c -o test2

run:
	ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer-3.5 ./test2

clean:
	rm -f *.o test1 test2


