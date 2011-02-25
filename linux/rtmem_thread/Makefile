CC_ARGS=-Wall -g -lrt

all: rtmem_mt

rtmem_mt: main.c
		powerpc-wrs-linux-gnu-ppc_e500v2-glibc_cgl-gcc main.c -o rtmem_mt $(CC_ARGS) 

test: rtmem_mt
		cp rtmem_mt /home/basberg/rtmem_mt

clean:
	rm -f rtmem_mt
