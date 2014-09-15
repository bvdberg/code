#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
    transpose a bit matrix of X rows of Y columns
*/

static const unsigned rows = 256;        // =  4*64
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

static inline void set(unsigned char* data, int val) {
}
#define setbit(x, y, v) (x |= (v << y))

// per 64-bit, try to cache output lines
// transpose 64x64bit blocks
static void transposeSquare(uint64_t* in, uint64_t* out) {
    uint64_t output[64];
    memset(output, 0, sizeof(output));
    for (unsigned i=0; i<64; i++) {
        const int offset = (columns / 64) * i;
        const uint64_t val = *(in + offset);
        for (int b=63; b>=0; --b) {
            uint64_t bit = (val >> b) & 1;
            output[b] |= (bit << i);
        }
    }
    uint64_t* op = out;
    for (unsigned i=0; i<64; i++) {
        *op = output[i];
        op += (rows/64);
    }
}

// always transpose buffer1 -> buffer2
static void transpose() {
    // cut up input into blocks of 64x64bit
    for (unsigned r=0; r<rows; r+=64) {
        for (unsigned c=0; c<columns/64; ++c) {
            int in_offset = (columns/64)*r + c;
            uint64_t* inp = buffer1 + in_offset;
            //printf("input block [%2d,%2d] = %d\n", c, r, in_offset);
            int out_offset = rows*c + r;
            uint64_t* outp = buffer2 + in_offset;
            //printf("output block [%2d,%2d] = %d\n", c, r, out_offset);
            transposeSquare(inp, outp);
            return;
        }
    }
    //transposeSquare(in, out);
}

// Compare with 64x64 byte transpose
// TODO use fact that most bits are 0? (get leftmost bit of uint32_t etc)

static void printInput(unsigned r, unsigned c) {
    printf("Input: %d,%d\n", r, c);
    uint64_t* ptr = buffer1 + (columns/64)*r + c;
    for (unsigned i=0; i<64; i++) {
        uint64_t val = *ptr;
        for (int j=63; j>=0; --j) {
            printf("%d", ((val >> j) & 1) == 1);
        }
        printf("\n");
        ptr += (columns/64);
    }
}

static void printOutput(unsigned r, unsigned c) {
    printf("Output: %d,%d\n", r, c);
    uint64_t* ptr = buffer2 + (rows/64)*c + r;
    for (unsigned i=0; i<64; i++) {
        uint64_t val = *ptr;
        //printf("value= %lu (0x%lx)\n", val, val);
        for (int j=63; j>=0; --j) {
            printf("%d", ((val >> j) & 1) == 1);
        }
        printf("\n");
        ptr += (rows/64);
    }
}

int main(int argc, const char *argv[])
{
    int size = rows * columns / 8;
    buffer1 = (uint64_t*)malloc(size);
    memset(buffer1, 0, size);
    buffer2 = (uint64_t*)malloc(size);
    memset(buffer2, 0, size);

    buffer1[63*columns/64] = 0xFFFFFFFFFFFFFFFFlu;

    printf("Input: %d x %d = %d bits (%d bytes)\n", rows, columns, rows*columns, size);
    uint64_t t1 = getCurrentTime();
    uint64_t tt1 = rdtsc();
    transpose();
    uint64_t tt2 = rdtsc();
    uint64_t t2 = getCurrentTime();
    printf("transpose(segment) took %lu usec (%lu cycles)\n", t2-t1, tt2-tt1);
    printInput(0, 0);
    printOutput(0, 0);

    // FIX: example broken (overwrites?)

    // TODO unit test with for all x,y == y,x

    return 0;
}

