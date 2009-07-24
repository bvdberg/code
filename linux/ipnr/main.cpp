#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void getAllIP() {
    int sck = socket(AF_INET, SOCK_DGRAM, 0);
    if(sck < 0)
    {
        perror("socket");
        return;
    }

    /* Query available interfaces. */
    struct ifconf ifc;
    char buf[1024];
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if(ioctl(sck, SIOCGIFCONF, &ifc) < 0)
    {
        perror("ioctl(SIOCGIFCONF)");
        return;
    }

    /* Iterate through the list of interfaces. */
    struct ifreq* ifr = ifc.ifc_req;
    int nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
    for(int i = 0; i < nInterfaces; i++)
    {
        struct ifreq *item = &ifr[i];
        printf("%s -> %s\n", item->ifr_name, inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr));
    }
}


void getIP(const char* ifname) {
    int sck = socket(AF_INET, SOCK_DGRAM, 0);
    if(sck < 0)
    {
        perror("socket");
        return;
    }

    /* Query available interfaces. */
    struct ifconf ifc;
    char buf[1024];
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if(ioctl(sck, SIOCGIFCONF, &ifc) < 0)
    {
        perror("ioctl(SIOCGIFCONF)");
        return;
    }

    /* Iterate through the list of interfaces. */
    struct ifreq* ifr = ifc.ifc_req;
    int nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
    for(int i = 0; i < nInterfaces; i++)
    {
        struct ifreq *item = &ifr[i];

        /* Show the device name and IP address */
        if (strcmp(ifname, item->ifr_name) == 0) {
            printf("%s -> %s  \n", item->ifr_name, inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr));
            return;
        }
    }
    printf("%s -> not found\n", ifname);
}


void getIP2() {
    char hostname[100];
    gethostname(hostname, sizeof(hostname));
    hostent* host = gethostbyname(hostname);
    if (host == NULL) {
        printf("unknown\n");
    } else {
        struct in_addr addr = *(struct in_addr *)(host->h_addr);
        printf("NEW %s  addrtype=%d\n", inet_ntoa(addr), host->h_addrtype);

        printf("name=%s  type=%d  length=%d\n", host->h_name, host->h_addrtype, host->h_length);

        for (int i=0; i<20; i++) {
            char* ptr = host->h_aliases[i];
            if (ptr != 0) printf("alias [%d] = '%s'\n", i, ptr);
        }
    }
}

int main(void) {
    getAllIP();
/*
    printf("\n--------------\n");
    getIP("eth0");
    getIP("eth1");
    getIP("lo");
    getIP("bla");
    printf("\n--------------\n");
    getIP2();
*/
    return 0;
}
