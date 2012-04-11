#include <stdio.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


static struct sockaddr_in sin;
static struct pollfd fds[20];
static int count = 0;
static int stop = 0;

int open_socket(int port) {
    sin.sin_family = AF_INET;
    sin.sin_port = htons (port);
    sin.sin_addr.s_addr = INADDR_ANY;
    memset(&(sin.sin_zero), '\0', 8);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd  == -1) {
        perror("socket");
        return -1;
    }

    int reuse = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1) {
        printf("error setting socket options\n");
        close(fd);
        return -1;
    }

    if (bind(fd, (struct sockaddr *) &sin, sizeof(struct sockaddr_in)) == -1) {
        close(fd);
        printf("socket already in use\n");
        return -1;
    }
    int MAX_PENDING_CONNECTIONS = 3;
    if (listen (fd, MAX_PENDING_CONNECTIONS) == -1) {
        close(fd);
        printf("error listening on socket\n");
        return -1;
    }

    return fd;
}


void printConnections() {
    int i;
    printf("%s()\n", __func__);
    for (i=0; i<count; i++) {
         printf(" %2d\n", fds[i].fd);
    }
}

void addConnection(int fd, short flags) {
    fds[count].fd = fd;
    fds[count].events = flags;
    count++;
    printConnections();
}

void acceptConnection(int fd) {
    struct sockaddr_in remote;
    unsigned int sin_size = sizeof(struct sockaddr_in);
    int newfd = accept(fd,(struct sockaddr *) &remote, &sin_size);
    if (newfd == -1) {
        printf("%s() ERROR\n", __func__);
        return;
    }              
    printf("received connection %d from %s\n", newfd, inet_ntoa(remote.sin_addr));
    addConnection(newfd, POLLIN);
}

void removeConnection(int index) {
    int fd = fds[index].fd;
    close(fd);
    int i;
    for (i=index; i<count-1; i++) {
        printf("copying %d -> %d\n", i, i+1);
        fds[i].fd = fds[i+1].fd;
        fds[i].events = fds[i+1].events;
        fds[i].revents = fds[i+1].revents;
    }
    count--;
    printf("lost connection %d\n", fd);
    printConnections();
}

void sigint(int num) {
    stop = 1;
}

int main(int argc, const char *argv[])
{
    int portnum = 6666;
    int fd = open_socket(portnum);
    if (fd == -1) return 0;
    printf("opening socket on port %d\n", portnum);

    addConnection(fd, POLLIN);
    int timeout_ms = 500;

    signal(SIGINT, sigint);
    while (stop == 0) {
        int ret = poll(fds, count, timeout_ms);
        if (ret != 0) printf("ret = %d\n", ret);
        if (fds[0].revents & POLLIN) {
            acceptConnection(fds[0].fd);
        }
        int i;
        for (i=1; i<count; i++) {
            if (fds[i].revents & POLLIN) {
                char buffer[64];
                int bytes = read(fds[i].fd, buffer, 64);
                if (bytes <= 0) { 
                    removeConnection(i);
                } else {
                    buffer[bytes] = 0;
                    buffer[bytes-2] = 0;    // strip off CR/LF
                    printf("%2d: %d bytes - '%s'\n", fds[i].fd, bytes, buffer);
                }
            }
        }
    }

    close(fd);
    return 0;
}

