CFLAGS=-Wall -Wextra

all: libfoo.so test

libfoo.so: mod1.c mod1.h
	gcc $(CFLAGS) -g -c -fPIC mod1.c
	gcc $(CFLAGS) -g -shared -Wl,-soname,libbar.so -o libfoo.so mod1.o

test: main.c
	gcc $(CFLAGS) -g main.c -o test libfoo.so

clean:
	rm -f *.o test *.so


