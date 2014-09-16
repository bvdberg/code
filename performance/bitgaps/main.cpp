#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static __inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
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


// TODO add minGap that has to be recorded on outparam (max 8), return numgaps > minGap
static void bit_gap(uint64_t* buf, unsigned offset, unsigned size, unsigned& distToLast) {
    uint64_t value = *buf;
    //printBuffer(0, &value, 1);
    uint64_t index = 63-offset;
    unsigned maxdist = distToLast;
    while (size--) {
        if ((value >> index) & 1) {
            if (distToLast > maxdist) maxdist = distToLast;
            distToLast = 0;
        }
        if (index == 0) {
            ++buf;
            value = *buf;
            index = 63;
            //printBuffer(0, &value, 1);
        } else {
            --index;
        }
        ++distToLast;
    }
    distToLast = maxdist;
}

int main(int argc, const char *argv[])
{
    unsigned offset = 20;
    unsigned overlap = 50;
    unsigned distToLast = 3;

    uint64_t buffer[2];
    buffer[0] = 0xf6fff00000ff5f0clu;
    buffer[1] = 0x0fffffffffffff0flu;
    printBuffer(0, &buffer[0], 2);

    bit_gap(&buffer[0], offset, overlap, distToLast);
    printf("maxdist=%d\n", distToLast);

    return 0;
}

