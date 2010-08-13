all: test test_arm

test: main.c
	gcc main.c -o test -Wall -lz

test_arm: main.c
	arm-none-linux-gnueabi-gcc main.c -o test_arm -Wall -lz -I /opt/toolchain/arm-2009q1/arm-none-linux-gnueabi/usr/include  -Wl,-rpath-link,/opt/toolchain/arm-2009q1/arm-none-linux-gnueabi/usr/lib -L/home/altenpts/oe/scratch/arago-tmp/sysroots/armv7a-none-linux-gnueabi/usr/lib
