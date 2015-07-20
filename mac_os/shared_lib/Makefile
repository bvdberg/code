CFLAGS=-Wall -Wextra -Wno-unused-parameter

all: libfoo.dylib test

libfoo.dylib: mod1.c mod1.h
	clang $(CFLAGS) -c -fPIC mod1.c
	clang $(CFLAGS) -g -dynamiclib  -Wl,-headerpad_max_install_names -o libfoo.dylib -install_name $(PWD)/libfoo.dylib mod1.o

libfoo.so_linux: mod1.c mod1.h
	gcc $(CFLAGS) -g -c -fPIC mod1.c
	gcc $(CFLAGS) -g -shared -Wl,-soname,libbar.so -o libfoo.so mod1.o

test: main.c
	gcc $(CFLAGS) -g main.c -o test libfoo.dylib

show:
	nm libfoo.dylib

clean:
	rm -f *.o test *.so *.dylib


