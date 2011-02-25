all: tickspeed tickspeed_linux

tickspeed: main.c
		gcc main.c -o tickspeed -Wall

tickspeed_linux: main.c
		powerpc-wrs-linux-gnu-ppc_e500v2-glibc_cgl-gcc main.c -o tickspeed_linux -Wall -g

test: tickspeed_linux
		cp tickspeed_linux /home/basberg/ts

clean:
	rm -f tickspeed tickspeed_linux
