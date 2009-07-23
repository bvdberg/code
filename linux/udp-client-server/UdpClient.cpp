#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

#include <stdexcept>
#include <iostream>

#include "UdpClient.h"
#include "Packet.h"

using namespace std;

UdpClient::UdpClient(const string& ipaddr, unsigned int port) : id(0) {
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        throw std::logic_error("no socket available");
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(0);
    
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr))) {
        close(fd);
        throw std::logic_error("cannot bind to socket");
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ipaddr.c_str());
    serverAddr.sin_port = htons(port);

    if (connect(fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(fd);
        throw std::logic_error("cannot connect to socket");
    }
}


UdpClient::~UdpClient() {
    close(fd);
}


void UdpClient::send() {
    const size_t size = 100;
    char buffer[size];
    for (unsigned int i=0; i<size; ++i) {
        fillPacket(buffer, size);
        if (::send(fd, buffer, size, 0) < 0) {
            cerr << "error sending packet " << i << endl;
        }
    }
}


void UdpClient::fillPacket(char* data, size_t size) {
    assert(size >= sizeof(Packet));
    memset(data, 0, size);
    Packet* header = (Packet*)data;
    ++id;
    header->id = id;
}
                                                              

int main(int argc, char* argv[]) {
    if (argc != 2) {
        UdpClient client("127.0.0.1", 12345);
        client.send();
    }  else {
        UdpClient client(argv[1], 12345);
        client.send();
    }
    return 0;
}



