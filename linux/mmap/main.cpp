#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZE 10

int main(int argc, char* argv[]) {
    if (argc != 2) { printf("usage: <file>\n"); return -1; }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) { perror("open"); return -1; }

    int pagesize = getpagesize();
    void* region = mmap(0, pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    if (region == MAP_FAILED) { perror("mmap"); return -1; }

    printf("region = %p\n", region);
    printf("%s", region);

    munmap(region, pagesize);
    close(fd);

    return 0;
}

