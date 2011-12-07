#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <strings.h>

//sendfile

static char* type2str(unsigned short type) {
    switch (type) {
    case 0x0800: return "IP";
    case 0x0806: return "ARP";
    }
    return "unknown";
}


static void printMac(unsigned char* data) {
     printf("%02x:%02x:%02x:%02x:%02x:%02x", data[0], data[1], data[2], data[3], data[4], data[5]);
}

static void printPacket(unsigned char* data, int size) {
    struct ethhdr* hdr = (struct ethhdr*)data;
    int i;
    printf("dest: "); printMac(hdr->h_dest);
    printf("   src: "); printMac(hdr->h_source);
    printf("   type: 0x%04x (%s)\n", ntohs(hdr->h_proto), type2str(ntohs(hdr->h_proto)));

    for (i=0; i<16; i++) {
         printf("%02x ", data[i]);
    }
    printf("\n");
}


int main(int argc, const char *argv[])
{
    int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (fd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_ll socket_address;
    bzero(&socket_address, sizeof(socket_address));
    socket_address.sll_family   = AF_PACKET;
    socket_address.sll_protocol = htons(ETH_P_ALL);
    int err = bind(fd, (struct sockaddr *) & socket_address, sizeof(socket_address));
    if (err) {
        perror("bind");
        return -1;
    }


    //recvfrom
    while (1) {
        char buffer[2000];
        int size = read(fd, buffer, 2000);
        if (size < 0) {
            perror("read");
            break;
        }
        if (size == 0) continue;
        printf("read %d bytes\n", size);
        printPacket((unsigned char*)buffer, size);
    }

    close(fd);
    return 0;
}
