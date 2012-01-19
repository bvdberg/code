#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

static int count = 0;

// TODO: use pcap_compile + pcap_filter (see http://www.tcpdump.org/pcap.html)


// NOTE: only works on Little Endian
struct RtpHeader
{
    //6 fields depend on Little Endian
    unsigned int csrccount:4;
    unsigned int extension:1;
    unsigned int padding:1;
    unsigned int version:2;
    unsigned int payloadtype:7;
    unsigned int marker:1;

    //these are converted later with ntohs/ntohl function
    u_int16_t sequencenumber;
    u_int32_t timestamp;
    u_int32_t ssrc;
} __attribute__ ((__packed__));


static char* mac2str(unsigned char* data) {
    static char buffer[4][24];
    static int index = 0;
    index++;
    if (index == 4) index = 0;
    char* cp = buffer[index];
    sprintf(cp, "%02x:%02x:%02x:%02x:%02x:%02x", data[0], data[1], data[2], data[3], data[4], data[5]);
    return cp;
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

static void receive_cb(u_char *useless, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    count++;
    if (pkthdr->caplen != pkthdr->len) {
        printf("weird sizes!\n");
        exit(-1);
    }
    //struct ether_header *eptr;  /* net/ethernet.h */
    struct ether_header* eptr = (struct ether_header*)packet;
    unsigned int type = ntohs(eptr->ether_type);
    if (type != 0x800) return;  // NO ip
    struct iphdr* hdr = (struct iphdr*)(&eptr[1]);
    if (hdr->protocol != IPPROTO_UDP) return; // NO UDP
    struct udphdr* hdr2 = (struct udphdr*)(&hdr[1]);
    printf("[%5d] %4d bytes", count, pkthdr->caplen);
    printf("  ETH[src=%s dst=%s]", mac2str(eptr->ether_shost), mac2str(eptr->ether_dhost));
    printf("  IP[src=%15s  dst=%15s  proto=%d]", ip2str(ntohl(hdr->saddr)), ip2str(ntohl(hdr->daddr)), hdr->protocol);
    printf("  UDP[src=%d  dst=%d]", ntohs(hdr2->source), ntohs(hdr2->dest));
    printf("\n");
    // TODO filter RTP
}

int main(int argc,char **argv)
{
    char errbuf[PCAP_ERRBUF_SIZE];
    //const u_char *packet;
    //struct pcap_pkthdr hdr;     /* pcap.h */
    //struct ether_header *eptr;  /* net/ethernet.h */

    char *dev = pcap_lookupdev(errbuf);
    if (dev == NULL)
    {
        printf("%s\n", errbuf);
        exit(1);
    }
    pcap_t* descr = pcap_open_live(dev, BUFSIZ, 0, -1, errbuf);
    if (descr == NULL)
    {
        printf("pcap_open_live(): %s\n", errbuf);
        exit(1);
    }

    pcap_loop(descr, 0, receive_cb, NULL);

    printf("done\n");
    return 0;
}

