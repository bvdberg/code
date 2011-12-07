#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <strings.h>

//sendfile
/*
    - dont use promiscuous mode (if not already)
    - filter out unwanted packets (in kernel?)
    - add client that sends using specific payload type and magic in payload
*/

static unsigned int count;

static char* type2str(unsigned short type) {
    switch (type) {
    case 0x0800: return "IP";
    case 0x0806: return "ARP";
    case 0x8100: return "VLAN TAG";
    case 0x88CC: return "LLDP";
    }
    return "unknown";
}


static void printMac(unsigned char* data) {
     printf("%02x:%02x:%02x:%02x:%02x:%02x", data[0], data[1], data[2], data[3], data[4], data[5]);
}

static void printPacket(unsigned char* data, int size) {
    struct ethhdr* hdr = (struct ethhdr*)data;
    printf("[%5d]", count);
    printf("   %4d bytes", size);
    printf("   dest: "); printMac(hdr->h_dest);
    printf("   src: "); printMac(hdr->h_source);
    unsigned short type = ntohs(hdr->h_proto);
    if (type == 0x8100) {   // VLAN tagging
        printf("  VLAN TAG!");
        // 4 bytes tag after macs, then 2 bytes type
    }
    printf("   type: 0x%04x (%s)\n", type, type2str(type));
}


int main(int argc, const char *argv[])
{
    int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); // with ethernet header
    //int fd = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ALL)); // without ethernet header

    //man packet(7)
    if (fd == -1) {
        perror("socket");
        return -1;
    }
#if 0
struct sockaddr_ll {
   unsigned short sll_family;   /* Always AF_PACKET */
   unsigned short sll_protocol; /* Physical layer protocol */
   int            sll_ifindex;  /* Interface number */
   unsigned short sll_hatype;   /* Header type */
   unsigned char  sll_pkttype;  /* Packet type */
   unsigned char  sll_halen;    /* Length of address */
   unsigned char  sll_addr[8];  /* Physical layer address */
};
#endif
    struct sockaddr_ll socket_address;
    bzero(&socket_address, sizeof(socket_address));
    socket_address.sll_family   = AF_PACKET;
    socket_address.sll_protocol = htons(ETH_P_ALL);
    //socket_address.sll_pkttype = PACKET_HOST;
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
        count++;
        printPacket((unsigned char*)buffer, size);
    }

    close(fd);
    return 0;
}

