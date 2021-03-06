#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char **argv) {
    int so_error;
    socklen_t len = sizeof(so_error);

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <ip> <port> <timeout_seconds>\n", argv[0]);
        return 1;
    }

    char* addr = argv[1];
    int port = atoi(argv[2]);
    int seconds = atoi(argv[3]);

    struct sockaddr_in addr_s;
    addr_s.sin_family = AF_INET; // utilizzo IPv4
    addr_s.sin_addr.s_addr = inet_addr(addr);
    addr_s.sin_port = htons(port);

    struct timespec tstart={0,0}, tend={0,0};
    clock_gettime(CLOCK_MONOTONIC, &tstart);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(fd, F_SETFL, O_NONBLOCK); // setup non blocking socket

    // make the connection
    int rc = connect(fd, (struct sockaddr *)&addr_s, sizeof(addr_s));
    if ((rc == -1) && (errno != EINPROGRESS)) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        close(fd);
        return 1;
    }
    if (rc == 0) {
        // connection has succeeded immediately
        clock_gettime(CLOCK_MONOTONIC, &tend);
        printf("socket %s:%d connected. It took %.5f seconds\n",
            addr, port, (((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec)));

        close(fd);
        return 0;
    } /*else {
        // connection attempt is in progress
    } */

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(fd, &fdset);
    struct timeval tv;
    tv.tv_sec = seconds;
    tv.tv_usec = 0;

    //rc = select(fd + 1, &fdset, NULL, NULL, &tv);
    rc = select(fd + 1, NULL, &fdset, NULL, &tv);
    switch(rc) {
    case 1: // data to read
        getsockopt(fd, SOL_SOCKET, SO_ERROR, &so_error, &len);
        if (so_error == 0) {
            clock_gettime(CLOCK_MONOTONIC, &tend);
            printf("socket %s:%d connected. It took %.5f seconds\n",
                addr, port, (((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec)));
            close(fd);
            return 0;
        } else { // error
            printf("socket %s:%d NOT connected: %s\n", addr, port, strerror(so_error));
        }
        break;
    case 0: //timeout
        fprintf(stderr, "connection timeout trying to connect to %s:%d\n", addr, port);
        break;
    }

    close(fd);
    return 0;
}
