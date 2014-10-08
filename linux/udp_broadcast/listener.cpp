#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>

#include "NetUtils.h"
#include "constants.h"

static int create_listen(const char*  ipaddr, unsigned int port)  {
    int fd;
    if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket");
        exit(-1);
    }

    u_int reuse = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("reusing ADDR failed");
        exit(1);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr))) {
        perror("bind");
        exit(-1);
    }

    int broadcast = 1;
    if (setsockopt(fd,SOL_SOCKET,SO_BROADCAST, &broadcast, sizeof(broadcast)) != 0) {
        perror("setsockopt - SO_SOCKET ");
        exit(-1);
    }

#if 0
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ipaddr);
    serverAddr.sin_port = htons(port);
    if (connect(fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("connect");
        exit(-1);
    }
#endif
    return fd;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s [iface]\n", argv[0]);
        return -1;
    }
    char* iface = argv[1];

    NetUtils::Interface interface;
    if (NetUtils::findInterface(iface, &interface)) {
        fprintf(stderr, "cannot find interface %s\n", iface);
        return -1;
    }
    printf("using interface %s  addr=%s  netmask=%s  bcast=%s\n", interface.name.c_str(),
        interface.addr.c_str(), interface.netmask.c_str(), interface.bcast.c_str());

    int fd = create_listen(interface.bcast.c_str(), PORT);

    while (1) {
        int len = 1500;
        char buffer[len];
        int flags = 0;
        struct sockaddr_in remote;
        unsigned sin_size = sizeof(struct sockaddr_in);
        memset(&remote, 0, sin_size);
        int res = recvfrom(fd, buffer, len, flags, (struct sockaddr*)&remote, &sin_size);
        if (res == -1) {
            perror("recvfrom");
            break;
        }
        buffer[res] = 0;
        const char* src_addr = inet_ntoa(remote.sin_addr);
        int src_port = ntohs(remote.sin_port);
        assert (remote.sin_family == AF_INET);
        printf("received %d bytes from %s:%d %s\n", res, src_addr, src_port, buffer);
    }
    close(fd);
    return 0;
}



