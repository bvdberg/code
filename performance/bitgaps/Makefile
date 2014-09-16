CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter -O2
MFLAGS = -march=corei7 -mavx -msse4 -mtune=barcelona
#MFLAGS = 

test: main.cpp
	g++ $(CFLAGS) main.cpp -lrt -o test

asm: main.cpp
	g++ -S $(MFLAGS) $(CFLAGS) main.cpp

clean:
	rm -f *.o test


