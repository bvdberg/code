#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>

static void find_ipnr(void) {
    struct in_addr ipnr = {};
    bool got_result = false;

    // first iterate network interfaces
    struct ifaddrs* addrs;
    getifaddrs(&addrs);

    struct ifaddrs* tmp = addrs;
    while (tmp) {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
            printf("%10s: %16s  0x%x\n", tmp->ifa_name, inet_ntoa(pAddr->sin_addr), tmp->ifa_flags);
            if (strncmp(tmp->ifa_name, "en", 2) == 0) {
                ipnr = pAddr->sin_addr;
                got_result = true;
            }
        }

        tmp = tmp->ifa_next;
    }
    freeifaddrs(addrs);

    if (got_result) {
        printf("my ipnr: %16s\n", inet_ntoa(ipnr));
    }
}

int main(int argc, const char *argv[])
{
    find_ipnr();
    return 0;
}

