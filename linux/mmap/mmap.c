#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

void printMemory(char* start, int size) {
    char* end = start + size;
    while(start < end) {
        printf("%c", *start);
        start++;
    }
    printf("\n");
}

void printProc() {
    pid_t pid = getpid();
    printf("/proc/%d/maps\n", pid);
}


int main() {
    int fd = open("file.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    int size = 4000;
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE;
    void* map = mmap(NULL, size, prot, flags, fd, 0);
    if (map == (void*)-1) {
        perror("mmap");
        return -1;
    }
    printf("map at %p\n", map);
    printMemory(map, 100);
    printProc();

    sleep(20);
    printf("closing file\n");
    close(fd);
    printMemory(map, 100);

    sleep(20);
    munmap(map, size);
    printf("unmapping memory\n");
    sleep(20);
    return 0;
}

