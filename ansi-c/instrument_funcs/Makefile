all: lib.so test

lib.so: lib.c
	gcc -fPIC lib.c -shared -nostdlib -o lib.so

test: main.c
	gcc -finstrument-functions main.c -o test

preload:
	LD_PRELOAD=./lib.so ./test

linker:
	gcc -fpic -g instrument.c -shared -o libinstrument.so -lbfd -liberty `pkg-config --cflags --libs glib-2.0`
	gcc main.c -o test -Xlinker -T ldorder

clean:
	rm -f *.o *.so test
