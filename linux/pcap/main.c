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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

static int count = 0;
void receive_cb(u_char *useless, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    count++;
    if (pkthdr->caplen != pkthdr->len) {
        printf("weird sizes!\n");
        exit(-1);
    }
    printf("[%5d] %d bytes  len=%d\n", count, pkthdr->caplen, pkthdr->len);
    //struct pcap_pkthdr hdr;     /* pcap.h */
    //struct ether_header *eptr;  /* net/ethernet.h */
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
