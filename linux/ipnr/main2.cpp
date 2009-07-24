#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdio.h>

int main(void) {
    int sck = socket(AF_INET, SOCK_DGRAM, 0);
    if(sck < 0)
    {
        perror("socket");
        return 0;
    }

    /* Query available interfaces. */
    struct ifconf ifc;
    char buf[1024];
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if(ioctl(sck, SIOCGIFCONF, &ifc) < 0)
    {
        perror("ioctl(SIOCGIFCONF)");
        return 0;
    }

    /* Iterate through the list of interfaces. */
    struct ifreq* ifr = ifc.ifc_req;
    int nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
    for(int i = 0; i < nInterfaces; i++)
    {
        struct ifreq *item = &ifr[i];
        printf("%s -> %s\n", item->ifr_name, inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr));
    }
    return 0;
}
