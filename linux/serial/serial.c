#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

#define _POSIX_SOURCE 1         //POSIX compliant source
#define FALSE 0
#define TRUE 1

volatile int stop=FALSE;

int main(int argc, const char *argv[])
{
    struct termios oldtio, newtio;       //place for old and new port settings for serial port

    long stopBITS = 0;   // 0,CstopB
    long PARITYON = 0;   // 0, PARENB
    long PARITY = PARODD;     // 0,PARODD,0

    //open the device(com port) to be non-blocking (read will return immediately)
    int fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0)
    {
        perror("error opening");
        exit(-1);
    }

    // allow the process to receive SIGIO
    fcntl(fd, F_SETOWN, getpid());
    // Make the file descriptor asynchronous (the manual page says only
    // O_APPEND and O_NONBLOCK, will work with F_SETFL...)
    fcntl(fd, F_SETFL, FASYNC);

    tcgetattr(fd,&oldtio); // save current port settings
    // set new port settings for canonical input processing
    newtio.c_cflag = B115200| CRTSCTS | CS8 | stopBITS | PARITYON | PARITY | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;       //ICANON;
    newtio.c_cc[VMIN]=1;
    newtio.c_cc[VTIME]=0;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);

    // loop while waiting for input. normally we would do something useful here
    while (!stop)
    {
        char buf[255];
        int res = read(fd, buf, sizeof(buf));
        if (res > 0)
        {
            int i;
            for (i=0; i<res; i++)  //for all chars in string
            {
                char In1 = buf[i];
                printf("%x ", In1);
            }
        }

    }
    tcsetattr(fd, TCSANOW, &oldtio);
    close(fd);
}

