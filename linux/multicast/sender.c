/*
 * sender.c -- multicasts "hello, world!" to a multicast group once a second
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define MULTICAST_PORT 12345
#define MULTICAST_IP "225.0.0.37"

int main(int argc, char* argv[])
{
    int fd;
    /* create what looks like an ordinary UDP socket */
    if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket");
        exit(1);
    }

    /* set up destination address */
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(MULTICAST_IP);
    addr.sin_port = htons(MULTICAST_PORT);

    unsigned char ttl = 1;
    if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) != 0) {
        perror("setsockopt");
        exit(-1);
    }

    int reuse = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
        perror("setsockopt");
        exit(-1);
    }

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr))) {
        perror("bind");
        exit(1);
    }

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(1);
    }

    u_char loop;
    socklen_t size;
    getsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, &size);
    printf("loop = %u\n", loop);

    printf("sending to %s, port %d\n", MULTICAST_IP, MULTICAST_PORT);

    uint32_t nr = 0;
    while (1) {
        char* message="Hello Multicast World!";
        char tmp[64];
        sprintf(tmp, "%s %d", message, nr);
        nr++;

        if (send(fd, tmp, strlen(tmp), 0) < 0) {
            perror("sendto");
            exit(1);
        }
        sleep(1);
    }

    return 0;
}

