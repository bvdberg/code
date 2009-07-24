#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <errno.h>

int main() {

    struct termios oldT, newT;
    char c;

    ioctl(0,TCGETS,&oldT);

    newT=oldT;
    newT.c_lflag &= ~ECHO;
    newT.c_lflag &= ~ICANON;
    newT.c_cc[VMIN] = 0;
    newT.c_cc[VTIME] = 0;

    ioctl(0, TCSETS, &newT);

    while (1)  {
        int res = read(0, &c, 1);
        if (res != 1 ) {
        } else {
            printf("read %c\n", c);
        }
    }

    ioctl(0,TCSETS,&oldT);
}

