CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter -O2

test: main.cpp FileMap.cpp
	g++ $(CFLAGS) main.cpp FileMap.cpp -I include --static -lsnappy -L lib -o test

clean:
	rm -f *.o test


