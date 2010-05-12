all: file.bin main.c
	od -A n -t x1 file.bin | sed -e 's/[0-9a-fA-F]\+/0x\U\0,/g' > file.h
	gcc main.c -o test

clean:
	rm -f test file.h
