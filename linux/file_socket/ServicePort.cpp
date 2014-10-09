#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <errno.h>

#include "ServicePort.h"

using namespace Luna;

ServicePort::ServicePort(bool creator_, const char* socketname_)
    : creator(creator_)
    , fd(0)
    , socketname(socketname_)
{
    if (creator) {
        if (unlink(socketname_) != 0) {
            if (errno != ENOENT) {
                perror("unlink");
                exit(-1);
            }
        }

        fd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
        if (fd == -1) {
            perror("socket");
            exit(-1);
        }

        if (fchmod(fd, S_IRUSR |S_IWUSR) != 0) {
            perror("fchmod");
            exit(-1);
        }

        struct sockaddr_un su;
        memset(&su, 0, sizeof(struct sockaddr_un));
        su.sun_family = AF_UNIX;
        strcpy(su.sun_path, socketname_);
        if (bind(fd, (struct sockaddr *)&su, sizeof(struct sockaddr_un)) == -1) {
            perror("bind");
            exit(-1);
        }
        if (listen(fd, 5) == -1) {
            perror("listen");
            exit(-1);
        }
    } else {
        fd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
        if (fd == -1) {
            perror("socket");
            exit(-1);
        }

        struct sockaddr_un su;
        memset(&su, 0, sizeof(struct sockaddr_un));
        su.sun_family = AF_UNIX;
        strcpy(su.sun_path, socketname_);
        if (connect(fd, (struct sockaddr *)&su, sizeof(struct sockaddr_un)) == -1) {
            perror("connect");
            exit(-1);
        }
    }
}

ServicePort::~ServicePort() {
    close(fd);
    if (creator) {
        if (unlink(socketname.c_str()) != 0) {
            perror("unlink");
        }
    }
}

int ServicePort::accept() {
    int new_fd = ::accept4(fd, NULL, NULL, SOCK_CLOEXEC);
    if (new_fd == -1) {
        perror("accept4");
        return -1;
    }
    return new_fd;
}

