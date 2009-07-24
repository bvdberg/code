run: TaskQueue.cpp runner.cpp TaskQueue.h
	@ g++ -c TaskQueue.cpp -O3 -Wall -Wextra
	@ g++ runner.cpp TaskQueue.o -o run -lpthread -O3 -Wall -Wextra
	@ rm -f *.o

clean:
	@ rm -f *.o run

