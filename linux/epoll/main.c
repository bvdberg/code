#include <stdio.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static int stop = 0;

static int createFifo()
{
    struct stat st;
    const char *fifo = "event.fifo";
    int socket;

    if (lstat (fifo, &st) == 0) {
        if ((st.st_mode & S_IFMT) == S_IFREG) {
            errno = EEXIST;
            perror("lstat");
            exit (1);
        }
    }

    unlink (fifo);
    if (mkfifo (fifo, 0600) == -1) {
        perror("mkfifo");
        exit (1);
    }

    /* Linux pipes are broken, we need O_RDWR instead of O_RDONLY */
    socket = open (fifo, O_RDWR | O_NONBLOCK, 0);
    if (socket == -1) {
        perror("open");
        exit (1);
    }
    fprintf(stderr, "Write data to %s\n", fifo);

    return socket;
}


void sigint(int num) {
    stop = 1;
}

int main(int argc, const char *argv[])
{
    int fd = createFifo();
/*
    epoll_create(16);
    epoll_ctl(6, EPOLL_CTL_ADD, 4, EPOLLIN|0x2000, {4,4});
    epoll_wait()
*/



//    int timeout_ms = 500;

    signal(SIGINT, sigint);
    while (stop == 0) {
    }

    close(fd);
    return 0;
}

