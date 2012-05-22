CFLAGS=-Wall -Wextra

test: main.c
	gcc $(CFLAGS) main.c -o test
	arm-none-linux-gnueabi-gcc $(CFLAGS) main.c -o test-arm

clean:
	rm -f *.o test


