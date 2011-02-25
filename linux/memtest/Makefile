memtest: memtest.c
		gcc memtest.c -o memtest -Wall

memtest_linux: memtest.c
		powerpc-wrs-linux-gnu-ppc_e500v2-glibc_cgl-gcc memtest.c -o memtest_linux -Wall

clean:
	rm -f memtest memtest_linux
