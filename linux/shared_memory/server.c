#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

//see man: shm_overview

#define SHM_NAME "data"
#define SHM_SIZE 4096

int main(int argc, const char *argv[])
{
    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    // NOTE: if ok, pseudo file /dev/shm/<name> is created
    fprintf(stderr, "fd=%d\n", fd);
    if (fd == -1) {
        perror("shm_open");
        // unlink if it existed
        shm_unlink(SHM_NAME);
        return -1;
    }

    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        shm_unlink(SHM_NAME);
        return -1;
    }

    void* map = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        shm_unlink(SHM_NAME);
        return -1;
    }
    fprintf(stderr, "map=%p\n", map);
    char* cp = (char*)map;
    unsigned i;
    for (i=0; i<100; i++) { cp[i] = i; }

    for (i=0; i<10; i++) {
        sleep(3);
        printf("checking begin: %d %d %d\n", cp[0], cp[1], cp[2]);
    }

    fprintf(stderr, "quit\n");
    munmap(map, SHM_SIZE);
    shm_unlink(SHM_NAME);
    close(fd);

    return 0;
}

