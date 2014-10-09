#include <stdio.h>

#include "ServicePort.h"
#include "constants.h"

using namespace Luna;

int main(int argc, const char *argv[])
{
    bool creator = false;
    if (argc == 2) creator = true;
    ServicePort port(creator, SERVICE_SOCKET);

    if (creator) {
        printf("created socket %s (fd=%d)\n", SERVICE_SOCKET, port.getFD());
    } else {
        // TEMP no error checking
        ssize_t written = write(port.getFD(), "aaa", 3);
        printf("write returned %ld\n", written);
        sleep (1);
        written = write(port.getFD(), "BBBB", 4);
        printf("write returned %ld\n", written);
        return 0;
    }


    while (1) {
        printf("waiting for connection\n");
        int fd = port.accept();
        printf("connected to socket (fd=%d)\n", fd);
        while (1) {
            char buffer[100];
            ssize_t numread = read(fd, buffer, 100);
            if (numread == -1) {
                perror("read");
                close(fd);
                break;
            }
            if (numread == 0) {
                printf("connection closed by other side\n");
                close(fd);
                break;
            }
            buffer[numread] = 0;
            printf("read %ld bytes [%s]\n", numread, buffer);
        }
    }

    return 0;
}

