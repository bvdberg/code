#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>

//sendfile


int main(int argc, const char *argv[])
{
    int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (fd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_ll socket_address;
    socket_address.sll_family   = AF_PACKET;
    socket_address.sll_protocol = htons(ETH_P_ALL);
    int err = bind(fd, (struct sockaddr *) & socket_address, sizeof(socket_address));
    if (err) {
        perror("bind");
        return -1;
    }


    while (1) {
        char buffer[2000];
        int num = read(fd, buffer, 2000);
        if (num < 0) {
            perror("read");
            break;
        }
        if (num == 0) continue;
        printf("read %d bytes\n", num);
    }

    close(fd);
    return 0;
}
