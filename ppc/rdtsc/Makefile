CC_ARGS=-Wall

all: rdtsc_sun rdtsc_linux

rdtsc_sun: main.c
		gcc main.c -o rdtsc_sun $(CC_ARGS)

rdtsc_linux: main.c
		powerpc-wrs-linux-gnu-ppc_e500v2-glibc_cgl-gcc main.c -o rdtsc_linux $(CC_ARGS)

clean:
	rm -f rdtsc_sun rdtsc_linux
