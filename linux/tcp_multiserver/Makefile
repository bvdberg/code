CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter

tcpserver: main.cpp TcpServer.cpp TcpServer.h Logger.cpp Logger.h
	g++ $(CFLAGS) main.cpp TcpServer.cpp Logger.cpp -o tcpserver

clean:
	rm -f *.o tcpserver


