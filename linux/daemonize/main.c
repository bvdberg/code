#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

static void daemonize()
{
    switch(fork()) {
    case 0: break;
    case -1:
        printf("fork failed\n");
        exit(-1);
    default:
        exit(0);
    }

    if (setsid() == -1) {
        perror("setsid");
        exit(-1);
    }

    switch(fork()) {
    case 0: break;
    case -1:
        printf("fork failed\n");
        exit(-1);
    default:
        exit(0);
    }
    printf("server [%d]\n", getpid());

    // sanitize stdfds
    close(0);
    close(1);
    close(2);
    int fd = 0;
    while (fd != 2) {
        fd = open("/dev/null", O_RDWR, 0);
        if (fd == -1) exit(errno);
    }
}


int main(int argc, const char *argv[])
{
    daemonize();
    printf("server\n");

    int i;
    int pid = getpid();
    for (i=0; i<40; i++) {
        printf("[%d] loop %d\n", pid, i);
        sleep(1);
    }
    return 0;
}

