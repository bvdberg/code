#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

#define _POSIX_SOURCE 1         //POSIX compliant source

volatile int stop=0;

static void sigint(int num) {
    stop = 1;
}


int main(int argc, const char *argv[]) {

    int fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NONBLOCK);
    //int fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("error opening");
        exit(-1);
    }

    struct termios oldtio, newtio;
    tcgetattr(fd, &oldtio);
    long stopBITS = 0;   // 0,CstopB
    long PARITYON = 0;   // 0, PARENB
    long PARITY = PARODD;     // 0,PARODD,0
    newtio.c_cflag = B115200 | CRTSCTS | CS8 | stopBITS | PARITYON | PARITY | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;       //ICANON;
    newtio.c_cc[VMIN]=1;
    newtio.c_cc[VTIME]=0;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW, &newtio);

    signal(SIGINT, sigint);

    while (!stop)
    {
        char buf[255];
        int rcount = read(fd, buf, sizeof(buf)-1);
        if (rcount == -1) {
            if (errno == EAGAIN) {
                usleep(10000);
                continue;
            }
            perror("read");
            return -1;
        }
        if (rcount == 0) continue;
        buf[rcount] = 0;
        printf("read %d bytes '%s'\n", rcount, buf);
    }
    tcsetattr(fd, TCSANOW, &oldtio);
    close(fd);
}

