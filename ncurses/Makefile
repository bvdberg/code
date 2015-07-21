CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter -std=c99
LDFLAGS=-lncurses

all: hello input menu scroll_menu window border worms dual_window

hello: hello.c
	gcc $(CFLAGS) hello.c -o hello $(LDFLAGS)

input: input.c
	gcc $(CFLAGS) input.c -o input $(LDFLAGS)

menu: menu.c
	gcc $(CFLAGS) menu.c -o menu $(LDFLAGS)

scroll_menu: scroll_menu.c
	gcc $(CFLAGS) scroll_menu.c -o scroll_menu $(LDFLAGS) -lmenu

window: window.c
	gcc $(CFLAGS) window.c -o window $(LDFLAGS)

border: border.c
	gcc $(CFLAGS) border.c -o border $(LDFLAGS)

worms: worms.c worms_helper.c worms_main.c worms.h worms_helper.h
	gcc $(CFLAGS) worms.c worms_helper.c worms_main.c -o worms $(LDFLAGS)

dual_window: dual_window.c
	gcc $(CFLAGS) dual_window.c -o dual_window $(LDFLAGS)

clean:
	rm -f *.o hello


