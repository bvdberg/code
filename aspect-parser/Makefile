all:
		@g++ -c AspectParser.cpp -Wall -Wextra
		@g++ parser.cpp AspectParser.o -Wall -Wextra -o parse
		@rm *.o

clean:
		@rm -f *.o parse

