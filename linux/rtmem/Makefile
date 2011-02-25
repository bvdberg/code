CC_ARGS=-Wall -g -lrt

all: rtmem

rtmem: main.c
		powerpc-wrs-linux-gnu-ppc_e500v2-glibc_cgl-gcc main.c -o rtmem $(CC_ARGS) 

test: rtmem
		cp rtmem /home/basberg/rtmem

clean:
	rm -f rtmem
