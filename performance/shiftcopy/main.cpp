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

//static const unsigned columns = 5*64;    // = 43*64
static const unsigned columns = 2752;    // = 43*64
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

static void printBuffer(const char* text, uint64_t* data, unsigned count) {
    if (text) printf("%s:\n", text);
    for (unsigned i=0; i<count; i++) {
        printf("  ");
        for (int j=63; j>=0; --j) {
            printf("%d", ((data[i] >> j) & 1) == 1);
        }
        printf("\n");
    }
}

#if 1
static uint64_t hmask[64];      //[63, 63-i] bits one

static void init() {
    uint64_t head = 0;
    for (int i=0; i<64; ++i) {
        //printf("mask [%2d]   h=0x%016lx\n", i, head);
        hmask[i] = head;
        head >>= 1;
        head |= (1lu<<63);

    }
}
#endif

// offset is offset in bits in input buffer. Can be > 64
// count = number of bits to be copied
// out must be able to handle distance shift
// padded output is guaranteed to be zero
static void shiftcopy(uint64_t* in, uint64_t* out, unsigned offset, unsigned count) {
    uint64_t* ip = in;
    uint64_t* op = out;
    const uint64_t hshift = offset;
    const uint64_t tshift = 64-hshift;
    while (offset >= 64) {
        ip++;
        offset -= 64;
    }

    uint64_t curval = *ip;
    while (count >= 64) {
        uint64_t nextval = *(ip+1);
#if 0
        printf("iter  [%d bits left]\n", count);
        printBuffer("  curval", &curval, 1);
        uint64_t h = (curval << hshift);
        uint64_t t = (nextval >> tshift);
        printBuffer("  head", &h, 1);
        printBuffer("  tail", &t, 1);
#endif
        *op = (curval << hshift) | (nextval >> tshift);
        ++op;
        ++ip;
        curval = nextval;
        count -= 64;
    }
    if (count > 0) {
        *op = ((curval << hshift) & hmask[count]);
        //printf("remaining %d\n", count);
        //printBuffer("  curval", &curval, 1);
        //printBuffer("  hmask", &hmask[count], 1);
    }
}



int main(int argc, const char *argv[])
{
    init();
    int size = columns / 8;
    printf("Input: %d bits (%d bytes)\n", columns, size);
    int loops = 1;
    if (argc > 1) {
        loops = atoi(argv[1]);
    }
    unsigned offset = 50;
    unsigned count = columns-offset;
    printf("shiftcopy offset=%u  count=%d (%d words)\n", offset, count, (count+63)/64);
    while (loops--) {
        buffer1 = (uint64_t*)malloc(size);
        memset(buffer1, 0, size);
        // TODO nice padding
        unsigned padding = 350;
        buffer2 = (uint64_t*)malloc(size + 2*padding);
        memset(buffer2, 0, size + 2*padding);

        buffer1[0] = 0x0fffffffffffff0flu;
        buffer1[1] = 0xafffffffffffff0flu;
        buffer1[2] = 0x0123456789abcdeflu;
        buffer1[3] = 0xafffffffffffff0flu;

        uint64_t t1 = getCurrentTime();
        uint64_t tt1 = rdtsc();
        shiftcopy(buffer1, buffer2, offset, count);
        uint64_t tt2 = rdtsc();
        uint64_t t2 = getCurrentTime();
        printf("shiftmove took %lu usec (%lu cycles)\n", t2-t1, tt2-tt1);
        if (argc > 2) {
            printBuffer("buffer1", buffer1, columns/64);
            printBuffer("buffer2", buffer2, columns/64);
        }
        free(buffer1);
        free(buffer2);
    }

    // FIX: transposed wrong way around -> depends on bit order!

    // TODO unit test with for all x,y == y,x

    return 0;
}

