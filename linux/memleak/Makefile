CC_ARGS=-Wall -g -lrt

all: memleak memleak_linux

memleak: memleak.cpp
		g++ memleak.cpp -o memleak $(CC_ARGS)

memleak_linux: memleak.cpp
		powerpc-wrs-linux-gnu-ppc_e500v2-glibc_cgl-g++ memleak.cpp -o memleak_linux $(CC_ARGS) 

test: memleak_linux
		cp memleak_linux /home/basberg/ml

clean:
	rm -f memleak memleak_linux
