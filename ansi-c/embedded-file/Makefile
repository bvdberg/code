all:
	objcopy --input binary --output elf32-i386 --binary-architecture i386 data.txt data.o
	echo "objects:"
	nm -a data.o
	g++ main.cpp data.o


