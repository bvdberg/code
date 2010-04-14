#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
    printf("server [%d]\n", getpid());
    close(0);
    close(1);
    close(2);
    int fd = open("/dev/null", O_RDWR, 0);
    fd = open("/dev/null", O_RDWR, 0);
    fd = open("/dev/null", O_RDWR, 0);
}


int main(int argc, const char *argv[])
{
    daemonize();
    printf("server\n");
    sleep(40);
    return 0;
}

