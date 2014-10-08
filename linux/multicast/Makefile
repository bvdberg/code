CFLAGS=-Wno-unused -Wno-unused-parameter

all: sender listener

sender: sender.c
	gcc $(CFLAGS) sender.c -o sender

listener: listener.c
	gcc $(CFLAGS) listener.c -o listener

clean:
	rm -f *.o sender listener


