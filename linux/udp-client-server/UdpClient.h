#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <string>

class UdpClient {
public:
    UdpClient(const std::string& ipaddr, unsigned int port);
    ~UdpClient();
    void send();
private:
    int fd;
    unsigned int id;

    void fillPacket(char* buffer, size_t size);
    UdpClient(const UdpClient& toBeCopied);
    UdpClient& operator=(const UdpClient& toBeCopied);
};

#endif

