CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter -std=c99
LDFLAGS=-lncurses

all: hello input window worms

hello: hello.c
	gcc $(CFLAGS) hello.c -o hello $(LDFLAGS)

input: input.c
	gcc $(CFLAGS) input.c -o input $(LDFLAGS)

window: window.c
	gcc $(CFLAGS) window.c -o window $(LDFLAGS)

worms: worms.c worms_helper.c worms_main.c worms.h worms_helper.h
	gcc $(CFLAGS) worms.c worms_helper.c worms_main.c -o worms $(LDFLAGS)

clean:
	rm -f *.o hello


