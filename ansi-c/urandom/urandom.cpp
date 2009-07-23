#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) perror("open");

    unsigned char buf[1];
    unsigned int total = 0;
    const int COUNT = 10000;
    for (int i=0; i<COUNT; i++) {
        ssize_t size = read(fd, buf, 1);
        if (size <= 0) break;
        printf("%d\n", buf[0]);
        total += buf[0];
    }
    printf("               total = %u  average = %u\n", total, total / COUNT);
    close(fd);
}

