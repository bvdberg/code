#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/time.h>

#include <mach/mach.h>
#include <mach/mach_time.h>

static uint64_t getCurrentTime() {
    struct timeval tp;
    if (gettimeofday(&tp, NULL) == -1) {
        perror("gettimeofday");
        return 0;
    }
    uint64_t result = tp.tv_sec;
    result *= 1000000;
    result += tp.tv_usec;
    return result;
}

int main(int argc, const char *argv[])
{
    uint64_t start = mach_absolute_time();
    usleep(100000);
    uint64_t end = mach_absolute_time();
    printf("100msec => diff  %lld nsec\n", end - start);

    uint64_t start2 = mach_absolute_time();
    sleep(1);
    uint64_t end2 = mach_absolute_time();
    printf("1sec    => diff %lld nsec\n", end2 - start2);

    uint64_t now = getCurrentTime();
    usleep(100000);
    uint64_t now2 = getCurrentTime();
    printf("100ms => diff %lld usec\n", now2 - now);
    return 0;
}

