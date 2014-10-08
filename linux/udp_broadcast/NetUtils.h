#ifndef NET_UTILS_H
#define NET_UTILS_H

#include <string>

class NetUtils {
public:
    struct Interface {
        std::string name;
        std::string addr;
        std::string netmask;
        std::string bcast;
        //std::string macnr;
        int mtu;
    };
    // returns -1 on error, 0 on ok
    static int findInterface(const std::string& iname, Interface* result);
};

#endif

