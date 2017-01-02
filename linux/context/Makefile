CFLAGS=-Wall -Wextra
#CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter -std=c99

test: context.c demo
	gcc $(CFLAGS) context.c -o test

demo: context_demo.c
	gcc $(CFLAGS) context_demo.c -o demo

clean:
	rm -f *.o test demo


