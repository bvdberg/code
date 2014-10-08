all: sender listener

sender: sender.cpp constants.h NetUtils.cpp NetUtils.h
	g++ -Wall -Wno-unused-parameter sender.cpp NetUtils.cpp -o sender -Wall -Wextra

listener: listener.cpp constants.h NetUtils.cpp NetUtils.h
	g++ -Wall -Wno-unused-parameter listener.cpp NetUtils.cpp -o listener -Wall -Wextra

clean:
	rm -rf sender listener
