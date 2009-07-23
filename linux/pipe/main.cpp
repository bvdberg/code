#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

__inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
}


int main() {
    printf("creating pipe\n");
    // NOTE: you can write 4096 bytes to pipe before it blocks (kernel constant)
    int filedes[2];     // 0 for reading, 1 for writing
    int result = pipe(filedes);
    if (result) {
        printf("ERROR: %s\n", strerror(errno));
        return -1;
    }

    const int bufsize = 4096;
    char* buf = (char*)malloc(bufsize);
    memset(buf, 0, bufsize);
    // NOTE: you can write 4096 bytes to pipe before it blocks (kernel constant)
    u_int64_t before = rdtsc();
    ssize_t size = write(filedes[1], buf, bufsize);
    u_int64_t after = rdtsc();
    if (size != bufsize) {
        printf("error writing: %s\n", strerror(errno));   
    }
    printf("written %d bytes, %llu cycles\n", size, after - before);

    before = rdtsc();
    size = read(filedes[0], buf, bufsize);
    after = rdtsc();
    printf("read %d bytes, %llu cycles\n", size, after - before);

    close(filedes[0]);
    close(filedes[1]);

    return 0;
}

