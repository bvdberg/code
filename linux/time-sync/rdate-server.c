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
    int SERVER_PORT = 9999;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("error opening socket\n");
        return -1;
    }

    struct sockaddr_in mijn_addr;
    mijn_addr.sin_family = AF_INET;
    mijn_addr.sin_port = htons(SERVER_PORT);
    mijn_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(mijn_addr.sin_zero), '\0', 8);

    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    if (bind(sockfd, (struct sockaddr *)&mijn_addr, sizeof(struct sockaddr)) != 0) {
        printf("error bind\n");
        return -1;
    }

    while (1) {
        if (listen(sockfd, 4) != 0)	{
            printf("error listening\n");
            return -1;
        }

        unsigned int sin_size = sizeof(struct sockaddr_in);
        struct sockaddr_in hun_addr;
        int new_fd = accept(sockfd, (struct sockaddr*)&hun_addr, &sin_size);
        if (new_fd == -1) {
            printf("error accepting new connection\n");
            return -1;
        }
        time_t now = time(0);
        int nrSent = send(new_fd, &now, sizeof(now), 0);
        printf("sent time (%d bytes)\n", nrSent);
        close(new_fd);
    }
    close(sockfd);
    return 0;
}

