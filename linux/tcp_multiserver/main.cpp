#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "TcpServer.h"

int main(int argc, const char *argv[])
{
    int port = 20000;
    if (argc == 2) {
        port = atoi(argv[1]);
    }
    TcpServer server(port);

    int fd = server.getFD();
    // TODO multi-connection
    while (1) {
        char* src_ip = 0;
        int conn_fd = server.accept(&src_ip);
        printf("new connection from %s\n", src_ip);

        while (1) {
            char buffer[1024];
            ssize_t numread = read(conn_fd, buffer, sizeof(buffer-1));
            if (numread < 0) {
                perror("read");
                return -1;
            }
            if (numread == 0) continue;
            buffer[numread] = 0;
            printf("read %d bytes: [", numread);
            for (int i=0; i<numread; i++) printf("%2x ", buffer[i]);
            printf("] (%s)\n", buffer);
        }
    }
    return 0;
}

