#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

#define SHM_NAME "data"
#define SHM_SIZE 4096

int main(int argc, const char *argv[])
{
    int fd = shm_open(SHM_NAME, O_RDWR, 0);
    // NOTE: if ok, pseudo file /dev/shm/<name> is created
    fprintf(stderr, "fd=%d\n", fd);
    if (fd == -1) {
        perror("shm_open");
        // unlink if it existed
        return -1;
    }

    void* map = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    fprintf(stderr, "map=%p\n", map);

    char* cp = (char*)map;
    unsigned i;
    for (i=0; i<100; i++) {
        printf("%d ", cp[i]);
        if (i%16 == 15) printf("\n");
    }
    printf("\n");

    for (i=0; i<10; i++) {
        printf("changing %d, %d -> %d\n", i, cp[i], cp[i] + 50);
        cp[i] = cp[i] + 50;
        sleep(3);
    }

    munmap(map, SHM_SIZE);
    close(fd);

    return 0;
}

