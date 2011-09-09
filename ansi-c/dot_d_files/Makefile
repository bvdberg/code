ARGS=-Wall

test: main.c lib.c
	gcc $(ARGS) -c lib.c -MMD -MF lib.c.d
	gcc $(ARGS) -c main.c -MMD -MF main.c.d
	gcc $(ARGS) main.o lib.o -o test

clean:
	rm -f *.o test *.d

