ARGS=-Wall

test: main.c lib.c
	gcc $(ARGS) -c lib.c -MMD -MF lib.c.d
	gcc $(ARGS) main.c lib.o -o test -MMD -MF main.c.d

clean:
	rm -f *.o test *.d

