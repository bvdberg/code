#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <strings.h>

#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

static unsigned int count;

static void print_line(unsigned char* buffer,  int offset, int num) {
    static char hexval[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    unsigned char line[80];
    memset(line, ' ', sizeof(line));
    line[79] = 0;
    int i;
    unsigned char* hex_cp = &line[0];
    unsigned char* char_cp = &line[16 * 3 + 4];
    for (i=0; i< num; i++) {
        unsigned char input = buffer[offset + i];
        *hex_cp++ = hexval[input >> 4];
        *hex_cp++ = hexval[input & 0x0F];
        hex_cp++;
        *char_cp++ = (isprint(input) ? input : '.');
    }
    printf("%s\n", line);
}

void print_buffer(unsigned char* buffer, int buffer_len) {
    int offset = 0;
    while (offset < buffer_len) {
        int remaining = buffer_len - offset;
        print_line(buffer, offset, (remaining >= 16) ? 16 : remaining);
        offset += 16;
    }
}


char* mac2str(unsigned char* data) {
    static char buffer[4][24];
    static int index = 0;
    index++;
    if (index == 4) index = 0;
    char* cp = buffer[index];
    sprintf(cp, "%02x:%02x:%02x:%02x:%02x:%02x", data[0], data[1], data[2], data[3], data[4], data[5]);
    return cp;
}

static char* type2str(unsigned short type) {
    switch (type) {
    case 0x0800: return "IP";
    case 0x0806: return "ARP";
    case 0x8100: return "VLAN TAG";
    case 0x88CC: return "LLDP";
    case 0xBABE: return "LLS";
    }
    return "unknown";
}

static char* ip2str(unsigned int addr) {
    static char buffer[4][20];
    static int index = 0;
    unsigned char* a = (unsigned char*)&addr;
    index++;
    if (index == 4) index = 0;
    char* cp = buffer[index];
    sprintf(cp, "%d.%d.%d.%d", a[3], a[2], a[1], a[0]);
    return cp;
}


static void printMac(unsigned char* data) {
     printf("%02x:%02x:%02x:%02x:%02x:%02x", data[0], data[1], data[2], data[3], data[4], data[5]);
}

static void printPacket(unsigned char* data, int size) {
    struct ethhdr* ehdr = (struct ethhdr*)data;
    printf("[%5d]", count);
    printf("   %4d bytes", size);
    printf("   dest: "); printMac(ehdr->h_dest);
    printf("   src: "); printMac(ehdr->h_source);
    unsigned short type = ntohs(ehdr->h_proto);
    if (type == 0x8100) {   // VLAN tagging
        printf("  VLAN TAG!");
        // 4 bytes tag after macs, then 2 bytes type
    }
    printf("   type: 0x%04x (%s)\n", type, type2str(type));
    if (type != 0x800) return;  // NO ip

    // ip
    struct iphdr* hdr = (struct iphdr*)(&ehdr[1]);
    if (hdr->protocol != IPPROTO_UDP) return; // NO UDP
    //unsigned int src = ntohl(hdr->saddr);
    //unsigned int dest = ntohl(hdr->daddr);
    //if (src_ip != 0 && src != src_ip) return;  // filter on src ip if set
    // udp
    struct udphdr* hdr2 = (struct udphdr*)(&hdr[1]);
    u_int16_t s_port = ntohs(hdr2->source);
    u_int16_t d_port = ntohs(hdr2->dest);
    //if (d_port < port_min || d_port > port_max) return; // filter on dest port
    //if (d_port != 10000) return;

//    printf("  ETH[src=%s dst=%s]", mac2str(ehdr->ether_shost), mac2str(ehdr->ether_dhost));
    printf("  IP[src=%15s  dst=%15s  proto=%d]", ip2str(ntohl(hdr->saddr)), ip2str(ntohl(hdr->daddr)), hdr->protocol);
    printf("  UDP[src=%d  dst=%d]", s_port, d_port);
    printf("RAW:\n");
    print_buffer(data, size);
    printf("\n");
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

