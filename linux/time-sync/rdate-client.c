#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    int DEST_PORT = 9999;

    if (argc != 2) {
        printf("usage: rdate-client <ipnr>\n");
        return -1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("error opening socket\n");
        return -1;
    }

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(DEST_PORT);
    dest_addr.sin_addr.s_addr = inet_addr(argv[1]);
    memset(&(dest_addr.sin_zero), '\0', 8);

    int res = connect(sockfd, (struct sockaddr* )&dest_addr, sizeof(struct sockaddr));
    if (res == -1) {
        printf("error connecting to %s\n", argv[1]);
        return -1;
    }

    time_t now;
    int recvbytes = recv(sockfd, &now, sizeof(now), 0);
    if (recvbytes != sizeof(now)) {
        printf("got erroneous response\n");
        return -1;
    }
    printf("setting time to (%d) -> %s\n", now, ctime(&now));
    stime(&now);

    close(sockfd);
    return 0;
}

