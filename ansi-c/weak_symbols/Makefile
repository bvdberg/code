all: with without

with: main.c extra.c
	gcc -c extra.c
	gcc main.c extra.o -Wall -o with

without: main.c
	gcc main.c -Wall -o without



