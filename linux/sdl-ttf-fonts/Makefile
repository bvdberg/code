# Makefile for showfont

CC = gcc
CFLAGS = -Wall -O
LIBS = -I/usr/include/SDL -lSDL_ttf
EXE = showfont

all: $(EXE)

showfont: showfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

clean:
	-rm *.o $(EXE)
