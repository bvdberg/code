#ifndef STREAMERSERVER_H
#define STREAMERSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

class StreamerServer {
public:
    StreamerServer(int port=19000);
    virtual ~StreamerServer();

    static void* loop(void* arg);

    virtual void handlePlayCmd(const char* ipaddr);
    virtual void handleStopCmd();
private:
    void executeCmd(char cmd, const char* ipaddr);

    int serverSocket_;
    struct sockaddr_in serveraddr_;

    pthread_t thread_;
    volatile bool running_;

};

#endif

