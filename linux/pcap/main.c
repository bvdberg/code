/**********************************************************************
* file:   testpcap2.c
* date:   2001-Mar-14 12:14:19 AM
* Author: Martin Casado
* Last Modified:2001-Mar-14 12:14:11 AM
*
* Description: Q&D proggy to demonstrate the use of pcap_loop
*
**********************************************************************/

#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/in.h>

static char* ip2str(unsigned int addr) {
    static char buffer[4][20];
    static int index = 0;
    unsigned char* a = (unsigned char*)&addr;
    index++;
    if (index == 4) index = 0;
    char* cp = buffer[index];
    sprintf(cp, "%d.%d.%d.%d", a[0], a[1], a[2], a[3]);
    return cp;
}

static int count = 0;
void receive_cb(u_char *useless, const struct pcap_pkthdr* pkthdr, const u_char* packet)
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
    printf("[%5d] %4d bytes  ", count, pkthdr->caplen);
    printf("src=%15s  dst=%15s  protocol=%d\n", ip2str(ntohl(hdr->saddr)), ip2str(ntohl(hdr->daddr)), hdr->protocol);

}

int main(int argc,char **argv)
{
    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* descr;
    //const u_char *packet;
    //struct pcap_pkthdr hdr;     /* pcap.h */
    //struct ether_header *eptr;  /* net/ethernet.h */

    dev = pcap_lookupdev(errbuf);
    if (dev == NULL)
    {
        printf("%s\n", errbuf);
        exit(1);
    }
    descr = pcap_open_live(dev, BUFSIZ, 0, -1, errbuf);
    if (descr == NULL)
    {
        printf("pcap_open_live(): %s\n", errbuf);
        exit(1);
    }

    pcap_loop(descr, 0, receive_cb, NULL);

    printf("done\n");
    return 0;
}
