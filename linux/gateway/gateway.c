#include <arpa/inet.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 8192

static struct in_addr nullAddr;
struct route_info {
    struct in_addr dstAddr;
    struct in_addr srcAddr;
    struct in_addr gateWay;
    char ifName[IF_NAMESIZE];
};

static int readRouteInfo(int sockFd, char *bufPtr, int seqNum, int pId) {
    struct nlmsghdr *nlHdr;
    int msgLen = 0;

    do {
        int readLen = recv(sockFd, bufPtr, BUFSIZE - msgLen, 0);
        if (readLen < 0) {
            perror("SOCK READ: ");
            return -1;
        }

        nlHdr = (struct nlmsghdr *)bufPtr;

        // Check if the header is valid
        if ((NLMSG_OK(nlHdr, readLen) == 0) || (nlHdr->nlmsg_type == NLMSG_ERROR))
        {
            perror("Error in received packet");
            return -1;
        }

        // Check if the its the last message
        if (nlHdr->nlmsg_type == NLMSG_DONE) {
            break;
        }
        else {
            // Else move the pointer to buffer appropriately
            bufPtr += readLen;
            msgLen += readLen;
        }

        // Check if its a multi part message
        if ((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0) {
            // return if its not
            break;
        }
    } while ((nlHdr->nlmsg_seq != seqNum) || (nlHdr->nlmsg_pid != pId));
    return msgLen;
}


// return found
static int parseRoutes(struct nlmsghdr *nlHdr, struct in_addr* result)
{
    struct rtmsg *rtMsg = (struct rtmsg *)NLMSG_DATA(nlHdr);

    // If the route is not for AF_INET or does not belong to main routing table, return.
    if ((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN)) return 0;

    // get the rtattr field
    struct rtattr *rtAttr = (struct rtattr *)RTM_RTA(rtMsg);
    int rtLen = RTM_PAYLOAD(nlHdr);
    struct route_info rtInfo;
    memset(&rtInfo, 0, sizeof(struct route_info));
    for (; RTA_OK(rtAttr,rtLen); rtAttr = RTA_NEXT(rtAttr,rtLen)) {
        switch (rtAttr->rta_type) {
        case RTA_OIF:
            if_indextoname(*(int *)RTA_DATA(rtAttr), rtInfo.ifName);
            break;
        case RTA_GATEWAY:
            rtInfo.gateWay.s_addr = *(u_int *)RTA_DATA(rtAttr);
            break;
        case RTA_PREFSRC:
            rtInfo.srcAddr.s_addr = *(u_int *)RTA_DATA(rtAttr);
            break;
        case RTA_DST:
            rtInfo.dstAddr.s_addr = *(u_int *)RTA_DATA(rtAttr);
            break;
        }
    }
    // check for default gateway
    if (rtInfo.dstAddr.s_addr == nullAddr.s_addr)  {
        //printf("%s\n", (char*)inet_ntoa(rtInfo.gateWay));
        *result = rtInfo.gateWay;
        return 1;
    }
    return 0;
}


static int find_gateway(struct in_addr* result) {
    int sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    char msgBuf[BUFSIZE];
    memset(msgBuf, 0, BUFSIZE);

    // point the header and the msg structure pointers into the buffer
    struct nlmsghdr *nlMsg = (struct nlmsghdr *)msgBuf;
    //struct rtmsg *rtMsg = (struct rtmsg *)NLMSG_DATA(nlMsg);

    // Fill in the nlmsg header
    nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)); // Length of message.
    nlMsg->nlmsg_type = RTM_GETROUTE; // Get the routes from kernel routing table .

    int msgSeq = 0;
    nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST; // The message is a request for dump.
    nlMsg->nlmsg_seq = msgSeq++; // Sequence of the message packet.
    nlMsg->nlmsg_pid = getpid(); // PID of process sending the request.

    // Send the request
    if (send(sock, nlMsg, nlMsg->nlmsg_len, 0) < 0) {
        printf("Write To Socket Failed...\n");
        return -1;
    }

    // Read the response
    int len = readRouteInfo(sock, msgBuf, msgSeq, getpid());
    if (len < 0) {
        printf("Read From Socket Failed...\n");
        return -1;
    }
    close(sock);

    // Parse and print the response
    for (; NLMSG_OK(nlMsg,len); nlMsg = NLMSG_NEXT(nlMsg,len)) {
        int found = parseRoutes(nlMsg, result);
        if (found) return 1;
    }
    return 0;
}


int main() {
    struct in_addr gateway;
    int found = find_gateway(&gateway);
    if (found == 1) {
        printf("found gateway %s\n", inet_ntoa(gateway));
    } else {
        printf("NO gateway found\n");
    }
    return 0;
}

