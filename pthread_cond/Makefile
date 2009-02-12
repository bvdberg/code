all:
	@g++ main.cpp -o test -lpthread -O2 -Wall -Wextra
	@g++ main.cpp -o test-timed -lpthread -lrt -DWITH_TIMEOUT -O2 -Wall -Wextra

clean:
	@rm -f test

