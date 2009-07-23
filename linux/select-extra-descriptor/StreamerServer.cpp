#include <stdio.h>
#include <iostream>

#include "StreamerServer.h"

using namespace std;

StreamerServer::StreamerServer(int port)
    : running_(true)
{
    if((serverSocket_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP) ) < 0)
        perror("StreamerServer: cannot create socket");

    memset(&serveraddr_, 0, sizeof(serveraddr_));
    serveraddr_.sin_family = AF_INET;
    serveraddr_.sin_addr.s_addr = htons(INADDR_ANY);
    serveraddr_.sin_port = htons(port);
    int flag = 1;
    if (setsockopt( serverSocket_, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag) ) < 0)
        perror("StreamerServer: set socket optiflags");

    struct linger linger;
    linger.l_onoff = 1;
    linger.l_linger =0;

    if (setsockopt(serverSocket_, SOL_SOCKET, SO_LINGER, &linger, sizeof(linger)) < 0)
        perror("StreamerServer: set socket optiflags");
    int on = 1;
    if (setsockopt(serverSocket_, IPPROTO_TCP, 1,(char*)&on, sizeof(on) ) < 0 )
        perror("StreamerServer: setting of TCP_NODELAY failed");

    if (bind(serverSocket_,(struct sockaddr *) &serveraddr_, sizeof(serveraddr_)) < 0)
        perror("StreamerServer: failed to bind");

    if (listen(serverSocket_, 5) < 0)
        perror("StreamerServer: failed to listen");

    if (pthread_create(&thread_, 0, StreamerServer::loop,(void *)this) != 0)
        perror("StreamerServer: thread creation failed");
}


void* StreamerServer::loop(void* arg) {
    StreamerServer* server =(StreamerServer*)arg;

    while (server->running_) {
        printf("loop\n");
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(server->serverSocket_, &rfds);
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        int result = select(server->serverSocket_ + 1,  &rfds, NULL, NULL, &tv);
        if (result > 0) {
            printf("result\n");
            struct sockaddr_in clientaddr;
            socklen_t client_size = sizeof(clientaddr);
            memset(&clientaddr, 0, client_size);
            int clientSocket = accept(server->serverSocket_, (struct sockaddr*)&clientaddr, &client_size);
            if (clientSocket > 0) {
                char cmd;
                int size = read(clientSocket, &cmd, sizeof(cmd));
                if (size < 0) {
                    std::cerr << "Read error: " << size << " sizeof(cmd): " << sizeof(cmd) << std::endl;
                } else if (size == sizeof(cmd))
                    server->executeCmd(cmd, inet_ntoa(clientaddr.sin_addr));
                else
                    std::cerr << "Read too many bytes actual size: " << size << " sizeof(cmd): " << sizeof(cmd) << std::endl;

                close(clientSocket);
            }
        }
    }
    return 0;
}


StreamerServer::~StreamerServer() {
    running_ = false;
    if (pthread_join(thread_, 0) != 0) perror("StreamerServer: Cannot join thread");
    close(serverSocket_);
}


void StreamerServer::executeCmd(char cmd, const char* ipaddr) {
    switch(cmd) {
    case 'p':
        handlePlayCmd(ipaddr);
        break;
    case 's':
        handleStopCmd();
        break;
    default :
        printf("%s WARNING: wrong command: %c\n", __PRETTY_FUNCTION__, cmd);
        break;
    }
}


void StreamerServer::handlePlayCmd(const char* ipaddr) {
    printf("StreamerServer: starting streaming to %s port 5000\n", ipaddr);
}


void StreamerServer::handleStopCmd() {
    printf("StreamerServer: stopping streaming\n");
}


int main() {
    {
        StreamerServer server;
        sleep(5);
        printf("STOPPING\n");
    }
    printf("stopped\n");
    sleep(2);
}

