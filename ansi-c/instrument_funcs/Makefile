all: libinstrument.so test

libinstrument.so: instrument.c
	gcc -fpic -g instrument.c -shared -o libinstrument.so -lbfd -liberty `pkg-config --cflags --libs glib-2.0`

test: main.c
	gcc -finstrument-functions main.c -o test

preload: test libinstrument.so
	LD_PRELOAD=./libinstrument.so ./test

clean:
	rm -f *.o *.so test
