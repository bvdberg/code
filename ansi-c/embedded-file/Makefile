test: data.txt main.c
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386 data.txt data.o
	echo "objects:"
	nm -a data.o
	gcc main.c data.o -o test

clean:
	rm -f data.o test

