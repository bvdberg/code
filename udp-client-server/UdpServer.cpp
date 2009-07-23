#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>

#include "UdpServer.h"
#include "Packet.h"
#include <stdexcept>

using namespace std;

UdpServer::UdpServer(unsigned int port) {
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        throw std::logic_error("no socket available");
    }

    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        close(fd);
        throw std::logic_error("cannot bind to socket");
    }
}


UdpServer::~UdpServer() {
    close(fd);   
}


void UdpServer::run() {
    const size_t MSGBUFSIZE = 1500;
    char msgbuf[MSGBUFSIZE];
    while (1) {
        struct sockaddr_in cliAddr; 
        socklen_t cliLen = sizeof(cliAddr);
        ssize_t recv_bytes = recvfrom(fd, msgbuf, MSGBUFSIZE, 0, reinterpret_cast<struct sockaddr*>(&cliAddr), &cliLen);
        if (recv_bytes < 0) {
            throw std::logic_error("cannot receive");
        }

        checker.addPacket(msgbuf, recv_bytes);
        checker.getStatus();
    }
}


int main() {
    UdpServer server(12345);
    server.run();

    return 0;
};


