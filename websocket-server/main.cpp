#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>

#include "WebSocketServer.h"

#define ANSI_BLUE     "\033[0;34m"
#define ANSI_NORMAL   "\033[0m"

#define COL_INFO ANSI_BLUE

using namespace ws;

int stop;
static WebSocketServer* g_server;

void sigint(int) {
    stop = 1;
}

class MyListener : public WebSocketServer::Listener {
public:
    MyListener() {}
    virtual ~MyListener() {}

    virtual void onNewConnection(const char* src_ip) {
        printf(COL_INFO "new connection from %s" ANSI_NORMAL"\n", src_ip);
    }
    virtual void onLostConnection() {
        printf("MyListener: %s()\n", __func__);
    }
    virtual void onReceiveData(uint32_t size, const uint8_t* data) {
        printf("MyListener: %s(size=%d)\n", __func__, size);
        //const char* text = "Good morning WebSockets!!";
        //sendData(strlen(text), (const uint8_t*)text);
        const char* large = "Good morging WebSockets. This a big story, to make sure " \
           " that the frame is larger then 126 bytes. Hopefully by now it is... Nope" \
           "still not big enough so I'll make up some more crap";
        g_server->sendData(strlen(large), (const uint8_t*)large);
    }
};


int main(int argc, const char *argv[])
{
    MyListener listener;
    // NOTE:  port 6000 doesn't work, but 7681 does!?
    int port = 7681;
    WebSocketServer server(port, listener);
    g_server = &server;

    int server_fd = server.getServerFd();

    signal(SIGINT, sigint);

    printf("listening on port %d\n", port);
    int i =0;
    while (!stop) {
#if 0
        fd_set rdfs;
        FD_ZERO(&rdfs);
        FD_SET(server_fd, &rdfs);
        int max_fd = server_fd;

        int conn_fd = server.getConnFd();
        if (conn_fd != -1) {
            FD_SET(conn_fd, &rdfs);
            if (conn_fd > max_fd) max_fd = conn_fd;
        }

        int res = select(max_fd + 1, &rdfs, NULL, NULL, NULL);
        if (res < 0) {
            if (errno == EINTR) continue;
            else {
                // TODO how to handle? sleep (to avoid busy-loop) + retry?
                const char* msg = strerror(errno);
                fprintf(stderr, "error on select: %s\n", msg);
                exit(-1);
            }
        }
        if (res > 0) {
            if (conn_fd != -1 && FD_ISSET(conn_fd, &rdfs)) {
                server.handleData();
            }
            if (FD_ISSET(server_fd, &rdfs)) {
                server.newConnection();
            }
        }
#else
        server.step();
        usleep(100000); // 10 Hz
        if (i % 10 == 9) {
            char msg[128];
            sprintf(msg, "message number %d", i);
            server.sendData(strlen(msg), (const uint8_t*)msg);
        }
        i++;
#endif
    }

    return 0;
}

