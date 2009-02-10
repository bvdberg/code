all:
	g++ -shared -fPIC spaceship.cpp -o libspaceship.so.1 -Wl,-soname=libspaceship.so.1
	ln -s -f libspaceship.so.1 libspaceship.so
	g++ testflight.cpp -L. -lspaceship -o testflight

exported:
	g++ -shared -fPIC spaceship.cpp -o libspaceship.so.1 -Wl,-soname=libspaceship.so.1 -Wl,--version-script=spaceship.expmap
	ln -s -f libspaceship.so.1 libspaceship.so
	g++ testflight.cpp -L. -lspaceship -o testflight
	

show:
	@nm -g -D -C --defined-only libspaceship.so.1

obj:
	@objdump -T -C libspaceship.so.1 | grep -v CXX | grep -v GLIB

