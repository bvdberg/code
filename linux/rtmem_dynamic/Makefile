CC_ARGS=-Wall -g -lrt

all: rtmem_dyn

rtmem_dyn: main.c
		powerpc-wrs-linux-gnu-ppc_e500v2-glibc_cgl-gcc main.c -o rtmem_dyn $(CC_ARGS) 

test: rtmem_dyn
		cp rtmem_dyn /home/basberg/rtmem_dyn

clean:
	rm -f rtmem_dyn
