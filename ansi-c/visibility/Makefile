CFLAGS=-Wall -Wextra -Wno-unused -O0

test: main.c
	gcc $(CFLAGS) main.c -o test

clean:
	@rm -f *.o test

show:
	@nm -g -C --defined-only test

