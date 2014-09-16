#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
    take bitarray (of uint64_t's) and copy and move
    TODO move to multiple buffers
*/

static const unsigned columns = 5*64;    // = 43*64
//static const unsigned columns = 2752;    // = 43*64
static uint64_t* buffer1;
static uint64_t* buffer2;

static __inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
}


static uint64_t getCurrentTime()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += (now.tv_nsec/1000);
    return now64;
}

static void printBuffer(uint64_t* data, unsigned count) {
    printf("buffer:\n");
    for (unsigned i=0; i<count; i++) {
        printf("  ");
        for (int j=63; j>=0; --j) {
            printf("%d", ((data[i] >> j) & 1) == 1);
        }
        printf("\n");
    }
}

// offset is offset in input buffer
// amount < 0 = shift left
static void shiftcopy(uint64_t* in, uint64_t* out, unsigned offset, int amount) {
}



int main(int argc, const char *argv[])
{
    int size = columns / 8;
    printf("Input: %d bits (%d bytes)\n", columns, size);
    int count = 1;
    if (argc > 1) {
        count = atoi(argv[1]);
    }
    while (count--) {
        buffer1 = (uint64_t*)malloc(size);
        memset(buffer1, 0, size);
        buffer2 = (uint64_t*)malloc(size);
        memset(buffer2, 0, size);

        buffer1[1] = 0x0123456789abcdef;
        buffer1[0] = 0xaa005500ff00aa00;

        uint64_t t1 = getCurrentTime();
        uint64_t tt1 = rdtsc();
        shiftcopy(buffer1, buffer2, 3, 20);
        uint64_t tt2 = rdtsc();
        uint64_t t2 = getCurrentTime();
        printf("shiftmove took %lu usec (%lu cycles)\n", t2-t1, tt2-tt1);
        if (1 || argc > 2) {
            printBuffer(buffer1, columns/64);
            printBuffer(buffer2, columns/64);
        }
        free(buffer1);
        free(buffer2);
    }

    // FIX: transposed wrong way around -> depends on bit order!

    // TODO unit test with for all x,y == y,x

    return 0;
}

