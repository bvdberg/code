#include <stdio.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>


int main(int argc, const char *argv[])
{
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons (6666);
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

    return 0;
}

