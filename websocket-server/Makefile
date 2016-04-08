CPPFLAGS=-O2 -Wall -Wextra -Wshadow -Wno-unused -Wno-unused-parameter -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS

SOURCES=\
    base64.cpp \
    sha-1.cpp \
    WebSocketServer.cpp \
    main.cpp

HEADERS=\
    base64.h \
    sha-1.h \
    WebSocketServer.h

all: ws_server

ws_server: $(SOURCES) $(HEADERS)
	g++ $(CPPFLAGS) $(SOURCES) -o ws_server

clean:
	rm -f *.o ws_server

