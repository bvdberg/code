#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

static int remote;  // socketFD
static int child;   // process ID

void parent_loop() {
    while (1) {
        unsigned char buffer[10];
        int num = read(remote, buffer, 10);
        if (num < 0) {
            perror("read");
            break;
        }
        if (num == 0) continue;
        buffer[num] = 0;
        printf("parent: read %d bytes (%s)\n", num, buffer);
        if (buffer[0] == 'Q') break;
    }
}


void child_loop() {
    int i = 0;
    while (i < 10) {
        char buffer[10];
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "msg[%02d]", i);
        int num = write(remote, buffer, 10);
        if (num != 10) {
            perror("write");
            break;
        }
        usleep(100000);
        i++;
    }
    write(remote, "Q", 1);
}


int main(int argc, const char *argv[])
{
    int pair[2];
    if (socketpair(AF_LOCAL, SOCK_DGRAM, PF_UNSPEC, pair) < 0) {
        perror("socketpair");
        return -1;
    }
    child = fork();
    if (child < 0) {
        perror("fork");
        return -1;
    }
    if (child == 0) {   // child
        remote = pair[0];
        close(pair[1]);
        printf("child: remote = %d\n", remote);
        child_loop();
    } else {        // parent
        remote = pair[1];
        close(pair[0]);
        printf("parent: remote = %d,  child = %d\n", remote, child);
        parent_loop();
    }

    return 0;
}
