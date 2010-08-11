#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
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
        // show macnr's
        struct ifreq ifr2;
        ifr2.ifr_addr.sa_family = AF_INET;
        strncpy(ifr2.ifr_name, "eth0", IFNAMSIZ-1);

        if (ioctl(sck, SIOCGIFHWADDR, &ifr2) != 0) {
            perror("ioctl(SIOCGIFHWADDR)");
            continue;
        }
        printf("  %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
         (unsigned char)ifr2.ifr_hwaddr.sa_data[0],
         (unsigned char)ifr2.ifr_hwaddr.sa_data[1],
         (unsigned char)ifr2.ifr_hwaddr.sa_data[2],
         (unsigned char)ifr2.ifr_hwaddr.sa_data[3],
         (unsigned char)ifr2.ifr_hwaddr.sa_data[4],
         (unsigned char)ifr2.ifr_hwaddr.sa_data[5]);



    }

    close(sck);
    return 0;
}
