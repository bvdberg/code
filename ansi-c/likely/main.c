#include <stdio.h>
#include <stdint.h>

volatile __inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
}

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

# ifndef likely
#  define likely(x) (__builtin_constant_p(x) ? !!(x) : __branch_check__(x, 1))
# endif
# ifndef unlikely
#  define unlikely(x)   (__builtin_constant_p(x) ? !!(x) : __branch_check__(x, 0))
# endif


int main(int argc, char* argv) {
    int num = (argc >= 2);
    uint64_t one = rdtsc();
    if (unlikely(num)) {
        printf("bla\n");
    }
    uint64_t two = rdtsc();
    printf("diff = %llu\n", two - one);
}

