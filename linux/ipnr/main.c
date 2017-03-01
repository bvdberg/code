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
    char buf[4096];
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if(ioctl(sck, SIOCGIFCONF, &ifc) < 0) {
        perror("ioctl(SIOCGIFCONF)");
        return 0;
    }

    /* Iterate through the list of interfaces. */
    struct ifreq* ifr = ifc.ifc_req;
    int nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
    int i;
    for(i = 0; i < nInterfaces; i++) {
        struct ifreq *item = &ifr[i];
        printf("%10s  %12s\t", item->ifr_name, inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr));

        // show netmask
        struct ifreq netmask;
        netmask.ifr_addr.sa_family = AF_INET;
        strncpy(netmask.ifr_name, item->ifr_name, IFNAMSIZ-1);
        if (ioctl(sck, SIOCGIFNETMASK, &netmask) != 0) {
            perror("ioctl(SIOCGIFNETMASK)");
            continue;
        }
        printf("  %16s", inet_ntoa(((struct sockaddr_in *)&netmask.ifr_addr)->sin_addr));

        // find broadcast
        struct ifreq bcast;
        memset(&bcast, 0, sizeof(struct ifreq));
        bcast.ifr_addr.sa_family = AF_INET;
        strncpy(bcast.ifr_name, item->ifr_name, IFNAMSIZ-1);
        if (ioctl(sck, SIOCGIFBRDADDR, &bcast) != 0) {
            perror("ioctl(SIOCGIFBRDADDR)");
            continue;
        }
        printf("  %16s", inet_ntoa(((struct sockaddr_in *)&bcast.ifr_broadaddr)->sin_addr));

        // show macnr
        struct ifreq hwaddr;
        hwaddr.ifr_addr.sa_family = AF_INET;
        strncpy(hwaddr.ifr_name, item->ifr_name, IFNAMSIZ-1);
        if (ioctl(sck, SIOCGIFHWADDR, &hwaddr) != 0) {
            perror("ioctl(SIOCGIFHWADDR)");
            continue;
        }
        unsigned char* cp = (unsigned char*)&hwaddr.ifr_hwaddr.sa_data[0];
        printf("  %.2x:%.2x:%.2x:%.2x:%.2x:%.2x", cp[0], cp[1], cp[2], cp[3], cp[4], cp[5]);

        // man netdevice(7)
        // show MTU
        struct ifreq macnr;
        bzero(&macnr, sizeof(macnr));
        macnr.ifr_addr.sa_family = AF_INET;
        strncpy(macnr.ifr_name, item->ifr_name, IFNAMSIZ-1);
        if (ioctl(sck, SIOCGIFMTU, &macnr) != 0) {
            perror("ioctl(SIOCGIFMTU)");
            continue;
        }
        int mtu = macnr.ifr_mtu;
        printf("  MTU=%d\n", mtu);

        //printf("%6s -> %12s\t", netmask.ifr_name, netmask.ifr_hwaddr.sa_data);
        // ioctl_list
    //    SIOCGIFNETMASK
    //    SIOCGIFBRDADDR

    }

    close(sck);
    return 0;
}
