#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

#include "TcpServer.h"
#include "Logger.h"

TcpServer::TcpServer(int port_)
    : port(port_)
{
    fd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);
    if (fd == -1) {
        fatal("opening TCP socket");
    }

    struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons((uint16_t)port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);

    int reuse = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    if (bind(fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) != 0) {
        fatal("TCP bind() failed", -EAGAIN);
    }
    if (::listen(fd, 4) != 0)	fatal("list() failed", -EAGAIN);
}

TcpServer::~TcpServer() {
    close(fd);
}

int TcpServer::accept(char** src_ipnr) {
    unsigned int sin_size = sizeof(struct sockaddr_in);
    struct sockaddr_in remote;
    int new_fd = ::accept4(fd, (struct sockaddr*)&remote, &sin_size, SOCK_CLOEXEC);
    if (new_fd == -1) {
        perror("accept4");
        return -1;
    }
    *src_ipnr = inet_ntoa(remote.sin_addr);
    return new_fd;
}

