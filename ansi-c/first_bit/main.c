#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

static unsigned int
msb32(register unsigned int x) {
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return (x & ~(x >> 1));
}

static inline uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return (uint64_t)hi << 32 | lo;
}

char values[32];
static void print_bits(uint32_t word) {
    uint32_t bits = word;
#if 0
    char output[64];
    char* cp = output;
    for (unsigned i=0; i<32; i++) {
        //int bit = (word & (1 << i));
        int bit = (word >> (31-i)) & 0x1;
       *cp++ = bit ? '1' : '0';
       if (i%4 == 3) *cp++ = ' ';
    }
    cp[-1] = 0;
    printf("0x%08X (%s)\n", word, output);
#endif

    // Results: For-loop still seems faster then builtin code below
#if 0
    for (unsigned i=0; i<31; ++i) {
        int bit = (word >> (31-i)) & 0x1;
        values[i] = bit;
    }

#else
    while (bits != 0) {
        const int zeroes = __builtin_clz(bits);
        //printf("    %d    %d  %d\n", zeroes, (bits & (1<<31)) != 0,  (bits >> 31) & 0x1);
        const int bit = 31 - zeroes;
        values[bit] = 1;
        //printf("  bit %d\n", bit);
        bits &= ~(1<<bit);
    }
#endif
}

int main(int argc, const char *argv[]) {
    // TODO measure speed
    uint32_t a = 1 << 3;
    printf("32: a = %10x (l,t) = %d,%d\n", a, __builtin_clz(a), __builtin_ctz(a));

    uint64_t b = 1 << 20;
    printf("64: b = %10x (l,t) = %d,%d\n",
           (unsigned)b,
           (unsigned)__builtin_clz(b),
           (unsigned)__builtin_ctz(b));

    int n = 1;
    for (;; n *= 2) {
        uint32_t msb;
        __asm__ __volatile__("bsrl %1,%0" : "=r"(msb) : "r"(n));
        printf("%12u : %2d", n, msb);
        printf("   CTZ: %2d  CLZ: %2d", __builtin_ctz(n), __builtin_clz(n));
        printf("\n");
        if (msb == 31) break;
    }

    print_bits(0x11110000);
    print_bits(0x00001111);
    print_bits(0xFF000000);
    print_bits(0x00000000);
    print_bits(0x00000001);
    print_bits(0xF00000F0);
    print_bits(0x80000000);
    print_bits(0x0F0F0000);
    print_bits(0xFFFFFFFF);

#if 1
    uint64_t t1 = rdtsc();
    //for (unsigned i=0; i<4000000000; ++i) {
    for (unsigned i=0; i<4000000; ++i) {
        print_bits(i);
    }
    uint64_t t2 = rdtsc();
    printf("TOOK %"PRIu64"\n", (t2 - t1) / 1000000LU);
#endif
    return 0;
}

