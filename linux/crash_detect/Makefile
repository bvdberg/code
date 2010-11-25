all: guard test-normal test-crash

guard: guard.c
	gcc guard.c -o guard -Wall

test-normal: main.c
	gcc main.c -o test-normal -Wall

test-crash: main.c
	gcc main.c -o test-crash -DCRASH -Wall
