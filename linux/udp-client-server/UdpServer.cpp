#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

#include "UdpServer.h"
#include "Packet.h"
#include <stdexcept>

using namespace std;
static void print_line(const unsigned char* buffer,  int offset, int num) {
    static char hexval[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    int i;
    unsigned char* hex_cp;
    unsigned char* char_cp;
    unsigned char line[80];
    memset(line, ' ', sizeof(line));
    line[79] = 0;
    hex_cp = &line[0];
    char_cp = &line[16 * 3 + 4];
    for (i=0; i< num; i++) {
        unsigned char input = buffer[offset + i];
        *hex_cp++ = hexval[input >> 4];
        *hex_cp++ = hexval[input & 0x0F];
        hex_cp++;
        *char_cp++ = (isprint(input) ? input : '.');
    }
    printf("%s\n", line);
}

static void print_buffer(const unsigned char* buffer, int buffer_len) {
    int offset = 0;
    while (offset < buffer_len) {
        int remaining = buffer_len - offset;
        print_line(buffer, offset, (remaining >= 16) ? 16 : remaining);
        offset += 16;
    }
}

UdpServer::UdpServer(unsigned int port) {
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        throw std::logic_error("no socket available");
    }

    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        close(fd);
        throw std::logic_error("cannot bind to socket");
    }
    printf("listening to port %d\n", port);
}


UdpServer::~UdpServer() {
    close(fd);   
}


void UdpServer::run() {
    const size_t MSGBUFSIZE = 1500;
    char msgbuf[MSGBUFSIZE];
    while (1) {
        struct sockaddr_in cliAddr; 
        socklen_t cliLen = sizeof(cliAddr);
        ssize_t recv_bytes = recvfrom(fd, msgbuf, MSGBUFSIZE, 0, reinterpret_cast<struct sockaddr*>(&cliAddr), &cliLen);
        if (recv_bytes < 0) {
            throw std::logic_error("cannot receive");
        }

        printf("received %d bytes from %s\n", (int)recv_bytes, inet_ntoa(cliAddr.sin_addr));
        print_buffer((const unsigned char*)msgbuf, (int)recv_bytes);
    }
}


int main() {
    UdpServer server(6701);
    server.run();

    return 0;
};


