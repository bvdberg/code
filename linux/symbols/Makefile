CFLAGS=-O3 -Wall -Wextra -Wno-unused -Wno-unused-parameter

all:
	g++ -shared -fPIC spaceship.cpp -o libspaceship.so.1 -Wl,-soname=libspaceship.so.1
	ln -s -f libspaceship.so.1 libspaceship.so
	g++ testflight.cpp -L. -lspaceship -o testflight

exported:
	g++ -shared -fPIC spaceship.cpp -o libspaceship.so.1 -Wl,-soname=libspaceship.so.1 -Wl,--version-script=spaceship.expmap -export-symbols=spaceship.sym
	#g++ -shared -fPIC spaceship.cpp -o libspaceship.so.1 -Wl,-soname=libspaceship.so.1 -export-symbols=spaceship.sym
	ln -s -f libspaceship.so.1 libspaceship.so
	g++ testflight.cpp -L. -lspaceship -o testflight

test:
	g++ -c -fPIC spaceship.cpp -o spaceship.o
	g++ -shared spaceship.o -o libspaceship.so.1 -Wl,-soname=libspaceship.so.1 -Wl,--version-script=spaceship.expmap -export-symbols=spaceship.sym
	#g++ -shared -fPIC spaceship.cpp -o libspaceship.so.1 -Wl,-soname=libspaceship.so.1 -export-symbols=spaceship.sym
	ln -s -f libspaceship.so.1 libspaceship.so
	g++ testflight.cpp -L. -lspaceship -o testflight

show:
	@nm -g -D -C --defined-only libspaceship.so.1

obj:
	@objdump -T -C libspaceship.so.1 | grep -v CXX | grep -v GLIB

clean:
	@rm -f testflight libspaceship* *.o
