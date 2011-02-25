CC_ARGS=-Wall -D__powerpc__

all: test_sun test_linux

test_sun: main.c
		gcc main.c -o test_sun $(CC_ARGS) 

test_linux: main.c
		powerpc-wrs-linux-gnu-ppc_e500v2-glibc_cgl-gcc main.c -o test_linux $(CC_ARGS)

clean:
	rm -f test_sun test_linux
