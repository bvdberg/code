#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <string>

#include "PacketChecker.h"

class UdpServer {
public:
    UdpServer(unsigned int port);
    ~UdpServer();
    void run();
private:
    int fd;
    unsigned int id;
    PacketChecker checker;

    UdpServer(const UdpServer& toBeCopied);
    UdpServer& operator=(const UdpServer& toBeCopied);
};

#endif

