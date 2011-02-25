CC_ARGS=-Wall -g -lrt -lpthread 

all: timertest

timertest: main.c
		powerpc-wrs-linux-gnu-ppc_e500v2-glibc_cgl-gcc main.c -o timertest $(CC_ARGS) 

test: timertest
		cp timertest /home/basberg/timertest

clean:
	rm -f timertest
