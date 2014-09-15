#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

static unsigned int
msb32(register unsigned int x)
{
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
        return(x & ~(x >> 1));
}

static inline uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
}


int main(int argc, const char *argv[])
{
    // TODO measure speed
    uint32_t a = 1<<3;
    printf("32: a = %10x (l,t) = %d,%d\n", a, __builtin_clz(a), __builtin_ctz(a));

    uint64_t b = 1<<20;
    printf("64: b = %10x (l,t) = %d,%d\n", (unsigned)b, (unsigned)__builtin_clz(b), (unsigned)__builtin_ctz(b));

  int n=1;
  for (;;n*=2) {
    uint32_t msb;
    __asm__ __volatile__ ("bsrl %1,%0" : "=r"(msb) : "r"(n));
    printf("%12u : %2d\n", n, msb);
    if (msb == 31) break;
  }
    return 0;
}

