#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include "NetUtils.h"

int NetUtils::findInterface(const std::string& iname, Interface* result) {
    assert(result);

    int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    // Query available interfaces
    struct ifconf ifc;
    char buf[1024];
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(fd, SIOCGIFCONF, &ifc) < 0) {
        perror("ioctl(SIOCGIFCONF)");
        close(fd);
        return -1;
    }

    // Iterate through the list of interfaces.
    struct ifreq* ifr = ifc.ifc_req;
    int nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
    int i;
    for (i = 0; i < nInterfaces; i++) {
        struct ifreq *item = &ifr[i];
        if (iname != item->ifr_name) continue;
        result->name = item->ifr_name;
        result->addr = inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr);
        //printf("%6s  %12s\t", item->ifr_name, inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr));

        // find netmask
        struct ifreq netmask;
        netmask.ifr_addr.sa_family = AF_INET;
        strncpy(netmask.ifr_name, item->ifr_name, IFNAMSIZ-1);
        if (ioctl(fd, SIOCGIFNETMASK, &netmask) != 0) {
            perror("ioctl(SIOCGIFNETMASK)");
            continue;
        }
        result->netmask = inet_ntoa(((struct sockaddr_in *)&netmask.ifr_netmask)->sin_addr);
        //printf("  %16s", inet_ntoa(((struct sockaddr_in *)&netmask.ifr_netmask)->sin_addr));

        // find broadcast
        struct ifreq bcast;
        memset(&bcast, 0, sizeof(struct ifreq));
        bcast.ifr_addr.sa_family = AF_INET;
        strncpy(bcast.ifr_name, item->ifr_name, IFNAMSIZ-1);
        if (ioctl(fd, SIOCGIFBRDADDR, &bcast) != 0) {
            perror("ioctl(SIOCGIFBRDADDR)");
            continue;
        }
        //printf("BROADCAST=%s\n", inet_ntoa(((struct sockaddr_in *)&bcast.ifr_broadaddr)->sin_addr));
        result->bcast = inet_ntoa(((struct sockaddr_in *)&bcast.ifr_broadaddr)->sin_addr);
#if 0
        // find macnr
        struct ifreq hwaddr;
        hwaddr.ifr_addr.sa_family = AF_INET;
        strncpy(hwaddr.ifr_name, item->ifr_name, IFNAMSIZ-1);
        if (ioctl(fd, SIOCGIFHWADDR, &hwaddr) != 0) {
            perror("ioctl(SIOCGIFHWADDR)");
            continue;
        }
        unsigned char* cp = (unsigned char*)&hwaddr.ifr_hwaddr.sa_data[0];
        printf("  %.2x:%.2x:%.2x:%.2x:%.2x:%.2x", cp[0], cp[1], cp[2], cp[3], cp[4], cp[5]);
#endif

#if 0
        // man netdevice(7)
        // find MTU
        struct ifreq macnr;
        bzero(&macnr, sizeof(macnr));
        macnr.ifr_addr.sa_family = AF_INET;
        strncpy(macnr.ifr_name, item->ifr_name, IFNAMSIZ-1);
        if (ioctl(fd, SIOCGIFMTU, &macnr) != 0) {
            perror("ioctl(SIOCGIFMTU)");
            continue;
        }
        int mtu = macnr.ifr_mtu;
        printf("  MTU=%d\n", mtu);
#endif

        close(fd);
        return 0;
    }

    close(fd);
    return -1;
}

