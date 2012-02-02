all: lib.so test

lib.so: lib.c
	gcc -fPIC lib.c -shared -nostdlib -o lib.so

test: main.c
	gcc -finstrument-functions main.c -o test

preload:
	LD_PRELOAD=./lib.so ./test

clean:
	rm -f *.o *.so test
