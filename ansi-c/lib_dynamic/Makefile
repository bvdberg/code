# Compiler/Linker/dynamic linker
CC = cc
LD = cc

# flags to compile object files that can be used in a dynamic library
CFLAGS = -fPIC -Wall -Wextra

# Flags to create a dynamic library.
DYNLINKFLAGS = -shared -nostdlib
# on some platforms, use '-G' instead.

all: mylib test

mylib: mylib.h mylib.c
	@echo 'building lib'
	$(CC) $(CFLAGS) -o mylib.o -c mylib.c
	$(CC) $(CFLAGS) -shared -o mylib.so mylib.o

test: main.c
	@echo 'building executable'
	$(CC) $(CFLAGS) main.c -c -o main.o
	$(CC) $(CFLAGS) main.o -rdynamic mylib.so -Wl,-rpath,${PWD} -o test

clean:
	rm -rf *.o *.so test
