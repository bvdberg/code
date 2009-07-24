#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

__inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
}


int main() {
    uint64_t one = rdtsc();
    uint64_t two = rdtsc();
    printf("diff = %llu\n", two - one);

    one = rdtsc();
    sleep(1);
    two = rdtsc();
    uint64_t cycles = two - one;
    printf("cycles/sec = %llu\n", cycles);

    return 0;
}

