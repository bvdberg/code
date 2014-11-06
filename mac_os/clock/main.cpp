#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

#include <mach/mach.h>
#include <mach/mach_time.h>

int main(int argc, const char *argv[])
{
    uint64_t start = mach_absolute_time();
    usleep(100000);
    uint64_t end = mach_absolute_time();
    printf("100usec => diff=%lld nsec\n", end - start);

    uint64_t start2 = mach_absolute_time();
    sleep(1);
    uint64_t end2 = mach_absolute_time();
    printf("1sec => diff=%lld nsec\n", end2 - start2);
    return 0;
}

