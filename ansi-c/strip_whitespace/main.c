#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>

/*
 *  strip whitespace from file:
 *  - empty lines
 *  - lines ending with spaces + newline
 *  - support both newline chars
 *  - print lines
 */


void parseFile(char* file, unsigned int size)
{
    printf("%s\n", file);
}

int main(int argc, const char *argv[])
{
    const char* filename = argv[1];

    int fd = open(filename, O_RDONLY);
    if (fd == -1) { perror("open"); return 0; }

    struct stat statbuf;
    int err = stat(filename, &statbuf);
    if (err) {
        perror("stat");
        close(fd);
        return 0;
    }
    unsigned int size = statbuf.st_size;

    void* map = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    if (map == MAP_FAILED) { perror("mmap"); return 0; }
    close(fd);

    parseFile(map, size);

    munmap(map, size);

    return 0;
}

