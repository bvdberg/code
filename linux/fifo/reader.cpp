/*
 * Compile with:
 * g++ event.cpp -levent
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <string>
#include <event.h>

using namespace std;


static inline char* itoa(unsigned int value) {
    static char str[11];
    str[10] = '\0';
    int  pos = 9;
    do {
        str[pos--] = (char) ((value % 10) + 0x30);
        value /= 10;
    } while (value != 0);
    return str + pos + 1;
}



int create_fifo(const char* fifo) {
	struct stat st;
	if (lstat (fifo, &st) == 0) {
		if ((st.st_mode & S_IFMT) == S_IFREG) {
			errno = EEXIST;
			perror("lstat");
			exit (1);
		}
	}

	unlink (fifo);
	if (mkfifo (fifo, 0600) == -1) {
		perror("mkfifo");
		exit (1);
	}
    
	int socket = open (fifo, O_RDWR | O_NONBLOCK, 0);
	if (socket == -1) {
		perror("open");
		exit (1);
	}
    return socket;
}


void delete_fifo(const char* fifo) {
    //TODO
}

class Reader {
public:
    Reader(string name_)
        : state(ACTIVE), name(name_)
    {
        printf("NEW Reader: %s\n", name.c_str());
        socket = create_fifo(name.c_str()); 
	    event_set(&evfifo, socket, EV_READ, Reader::callback, this);
        reschedule(10);
    }
    void handleEvent(short event) {
        if (event == EV_TIMEOUT) {
            printf("%s: timeout: ", name.c_str());
            switch(state) {
            case ACTIVE:
                printf("becoming idle\n");
                state = IDLE;
                break;
            case IDLE:
                printf("disconnecting\n");
                exit(0);
                break;
            }
        }

        if (event == EV_READ) {
            if (state == IDLE) {
                printf("becoming active\n");
                state = ACTIVE;
            }
            char buf[255];
            memset(buf, 0, 255);
            int len = read(socket, buf, sizeof(buf) - 1);
            buf[len] = '\0';
            printf("%s: read: '%s'\n", name.c_str(), buf);
            if (strcmp("close\n", buf) == 0) {
                printf("Reader: closing\n");
                //TODO close and delete fifo
            }
        }
        reschedule(10);
    }
    void reschedule(int seconds) {
	    struct timeval tv;
    	tv.tv_sec = seconds;
        tv.tv_usec = 0;
    	event_add(&evfifo, &tv);
    }
    static void callback(int, short event, void* arg) {
        Reader* reader = (Reader*) arg;
        reader->handleEvent(event);
    }
private:
    enum STATE { ACTIVE, IDLE };
    STATE state;
    std::string name;
    int socket;
	struct event evfifo;
};

class Server {
public:
    Server(string name_)
        : name(name_), readerID(0)
    {
        socket = create_fifo(name.c_str()); 
	    event_set(&evfifo, socket, EV_READ, Server::callback, this);
        reschedule(2);
    }
    void handleEvent(short event) {
        printf("%s event\n", name.c_str());
        if (event == EV_TIMEOUT) {
            printf("Timeout\n");
            // TODO check if terminate, terminate all readers
        }

        if (event == EV_READ) {
            char buf[255];
            memset(buf, 0, 255);
            int len = read(socket, buf, sizeof(buf) - 1);
            buf[len] = '\0';
            printf("Server: read: '%s'\n", buf);
            if (strcmp(buf, "new\n") ==0) {
                printf("Server: spawning new Reader\n");
                new Reader("reader" + string(itoa(readerID)));
                readerID++;
                // TODO add to list
            }
            if (strcmp(buf, "close\n") ==0) {
                printf("Server: closing all connections\n");
                // TODO close all readers
            }
        }
        reschedule(2);
    }
    static void callback(int, short event, void* arg) {
        Server* server = (Server*) arg;
        server->handleEvent(event);
    }
    void reschedule(int seconds) {
	    struct timeval tv;
    	tv.tv_sec = seconds;
        tv.tv_usec = 0;
    	event_add(&evfifo, &tv);
    }
private:
    std::string name;
    int socket;
	struct event evfifo;
    unsigned int readerID;
};


int main () {
	event_init();

    Server server("server1");
//    Reader reader1("reader1");
//    Reader reader2("reader2");

	event_dispatch();
	return 0;
}

