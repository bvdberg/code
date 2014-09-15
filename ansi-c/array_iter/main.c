#include <stdio.h>
#include <stdint.h>

static inline uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
}
#define SIZE_X 10
#define SIZE_Y 100
unsigned buffer[SIZE_X][SIZE_Y];        // in memory: Y's are sequencial!
// NOTE: fastest iterate is X outer, Y inner loop

int main(int argc, const char *argv[])
{
    fprintf(stderr, "buffer = %p\n", &buffer);
    uint64_t t1 = rdtsc();
    uint64_t total = 0;
    for (unsigned x=0; x<SIZE_X; ++x) {
        for (unsigned y=0; y<SIZE_Y; ++y) {
            buffer[x][y] = total++;
        }
    }
    uint64_t t2 = rdtsc();
    fprintf(stderr, "diff = %lu\n", t2 -t1);
    fprintf(stderr, "total = %lu\n", total);

    // check order, should be 0-999
    unsigned* cp = (unsigned*)buffer;
    for (unsigned i=0; i<SIZE_X*SIZE_Y; ++i) {
        //printf("[%3d] %3d\n", i, cp[i]);
    }

    return 0;
}

