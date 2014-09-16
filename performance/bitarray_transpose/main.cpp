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

// per 64-bit, try to cache output lines
// transpose 64x64bit blocks
static void transposeSquare(uint64_t* in, uint64_t* out) {
    uint64_t output[64];
    memset(output, 0, sizeof(output));
    for (unsigned i=0; i<64; ++i) {
        const int offset = (columns / 64) * i;
        const uint64_t val = *(in + offset);
        for (int b=63; b>=0; --b) {
            uint64_t bit = (val >> b) & 1;
            output[b] |= (bit << i);
        }
    }
    uint64_t* op = out;
    for (unsigned i=0; i<64; ++i) {
        *op = output[i];
        //printf("out [%2d] = %d\n", i, (int)(op-buffer2));
        op += (rows/64);
    }
}

static void transposeByOutput(uint64_t* in, uint64_t* out) {
    uint64_t ip[64];
    for (unsigned i=0; i<64; ++i) {
        const int offset = (columns / 64) * i;
        ip[i] = *(in + offset);
    }
    uint64_t* op = out;
    for (unsigned i=0; i<64; ++i) {
        *op =
            ((ip[ 0] >> i) & 0x1lu) << 63 |
            ((ip[ 1] >> i) & 0x1lu) << 62 |
            ((ip[ 2] >> i) & 0x1lu) << 61 |
            ((ip[ 3] >> i) & 0x1lu) << 60 |
            ((ip[ 4] >> i) & 0x1lu) << 59 |
            ((ip[ 5] >> i) & 0x1lu) << 58 |
            ((ip[ 6] >> i) & 0x1lu) << 57 |
            ((ip[ 7] >> i) & 0x1lu) << 56 |
            ((ip[ 8] >> i) & 0x1lu) << 55 |
            ((ip[ 9] >> i) & 0x1lu) << 54 |
            ((ip[10] >> i) & 0x1lu) << 53 |
            ((ip[11] >> i) & 0x1lu) << 52 |
            ((ip[12] >> i) & 0x1lu) << 51 |
            ((ip[13] >> i) & 0x1lu) << 50 |
            ((ip[14] >> i) & 0x1lu) << 49 |
            ((ip[15] >> i) & 0x1lu) << 48 |
            ((ip[16] >> i) & 0x1lu) << 47 |
            ((ip[17] >> i) & 0x1lu) << 46 |
            ((ip[18] >> i) & 0x1lu) << 45 |
            ((ip[19] >> i) & 0x1lu) << 44 |
            ((ip[20] >> i) & 0x1lu) << 43 |
            ((ip[21] >> i) & 0x1lu) << 42 |
            ((ip[22] >> i) & 0x1lu) << 41 |
            ((ip[23] >> i) & 0x1lu) << 40 |
            ((ip[24] >> i) & 0x1lu) << 39 |
            ((ip[25] >> i) & 0x1lu) << 38 |
            ((ip[26] >> i) & 0x1lu) << 37 |
            ((ip[27] >> i) & 0x1lu) << 36 |
            ((ip[28] >> i) & 0x1lu) << 35 |
            ((ip[29] >> i) & 0x1lu) << 34 |
            ((ip[30] >> i) & 0x1lu) << 33 |
            ((ip[31] >> i) & 0x1lu) << 32 |
            ((ip[32] >> i) & 0x1lu) << 31 |
            ((ip[33] >> i) & 0x1lu) << 30 |
            ((ip[34] >> i) & 0x1lu) << 29 |
            ((ip[35] >> i) & 0x1lu) << 28 |
            ((ip[36] >> i) & 0x1lu) << 27 |
            ((ip[37] >> i) & 0x1lu) << 26 |
            ((ip[38] >> i) & 0x1lu) << 25 |
            ((ip[39] >> i) & 0x1lu) << 24 |
            ((ip[40] >> i) & 0x1lu) << 23 |
            ((ip[41] >> i) & 0x1lu) << 22 |
            ((ip[42] >> i) & 0x1lu) << 21 |
            ((ip[43] >> i) & 0x1lu) << 20 |
            ((ip[44] >> i) & 0x1lu) << 19 |
            ((ip[45] >> i) & 0x1lu) << 18 |
            ((ip[46] >> i) & 0x1lu) << 17 |
            ((ip[47] >> i) & 0x1lu) << 16 |
            ((ip[48] >> i) & 0x1lu) << 15 |
            ((ip[49] >> i) & 0x1lu) << 14 |
            ((ip[50] >> i) & 0x1lu) << 13 |
            ((ip[51] >> i) & 0x1lu) << 12 |
            ((ip[52] >> i) & 0x1lu) << 11 |
            ((ip[53] >> i) & 0x1lu) << 10 |
            ((ip[54] >> i) & 0x1lu) <<  9 |
            ((ip[55] >> i) & 0x1lu) <<  8 |
            ((ip[56] >> i) & 0x1lu) <<  7 |
            ((ip[57] >> i) & 0x1lu) <<  6 |
            ((ip[58] >> i) & 0x1lu) <<  5 |
            ((ip[59] >> i) & 0x1lu) <<  4 |
            ((ip[60] >> i) & 0x1lu) <<  3 |
            ((ip[61] >> i) & 0x1lu) <<  2 |
            ((ip[62] >> i) & 0x1lu) <<  1 |
            ((ip[63] >> i) & 0x1lu);
        op += (rows/64);
    }
}

// always transpose buffer1 -> buffer2
static void transpose() {
    // cut up input into blocks of 64x64bit
    for (unsigned r=0; r<rows/64; ++r) {
        for (unsigned c=0; c<columns/64; ++c) {
            const int in_offset = (columns/64)*r + c;
            uint64_t* inp = buffer1 + in_offset;

            const int out_offset = rows*c + r;
            uint64_t* outp = buffer2 + out_offset;
            //printf("transpose c=%d r=%d  offset=%d\n", c, r, out_offset);
            //transposeSquare(inp, outp);
            transposeByOutput(inp, outp);
            return;
        }
    }
}

static void printInput(unsigned r, unsigned c) {
    const uint64_t offset = (columns*r) + c;
    printf("Input: %d,%d (offset=0x%lx)\n", r, c, offset);
    uint64_t* ptr = buffer1 + offset;
    for (unsigned i=0; i<64; i++) {
        uint64_t val = *ptr;
        printf("  ");
        for (int j=63; j>=0; --j) {
            printf("%d", ((val >> j) & 1) == 1);
        }
        printf("\n");
        ptr += (columns/64);
    }
}

static void printOutput(unsigned r, unsigned c) {
    const uint64_t offset = rows*c + r;
    printf("Output: %d,%d (offset=0x%lx)\n", r, c, offset);
    uint64_t* ptr = buffer2 + offset;
    for (unsigned i=0; i<64; i++) {
        uint64_t val = *ptr;
        printf("  ");
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

    buffer1[4*columns/64] = 0xFFFFFFFFFFFFFFFFlu;
    for (int i=0; i<64; i++) {
        buffer1[(columns/64)*i] |= 0x8000000000000000;
    }

    printf("Input: %d x %d = %d bits (%d bytes)\n", rows, columns, rows*columns, size);
    uint64_t t1 = getCurrentTime();
    uint64_t tt1 = rdtsc();
    transpose();
    //transposeSquare(buffer1, buffer2);
    //transposeByOutput(buffer1, buffer2);
    uint64_t tt2 = rdtsc();
    uint64_t t2 = getCurrentTime();
    printf("transpose(segment) took %lu usec (%lu cycles)\n", t2-t1, tt2-tt1);
    if (argc > 1) {
        printInput(0, 0);
        printOutput(0, 0);
    }

    // FIX: transposed wrong way around -> depends on bit order!

    // TODO unit test with for all x,y == y,x

    return 0;
}

