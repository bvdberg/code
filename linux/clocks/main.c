#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

uint64_t time1() {
    struct timeval now;
    gettimeofday(&now, NULL);
    uint64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += now.tv_usec;
    return now64;
}

uint64_t time2() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += now.tv_nsec/1000;
    return now64;
}

uint64_t time3() {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    uint64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += now.tv_nsec/1000;
    return now64;
}

void resolutions() {
    struct timespec res;
    clock_getres(CLOCK_REALTIME, &res);
    printf("REALTIME  = %ld.%09lld\n", res.tv_sec, res.tv_nsec);  

    clock_getres(CLOCK_MONOTONIC, &res);
    printf("MONOTONIC = %ld.%09lld\n", res.tv_sec, res.tv_nsec);  

    clock_getres(CLOCK_PROCESS_CPUTIME_ID, &res);
    printf("PROCESS_CPUTIME_ID = %ld.%09lld\n", res.tv_sec, res.tv_nsec);  
}


int main() {
    resolutions();

    uint64_t t1 = time1();
    uint64_t t2 = time2();
    uint64_t diff = t1 - t2;
    printf("t1=%lld  t2=%lld  diff=%lld\n", t1, t2, diff);

    sleep(1);

    t1 = time1();
    t2 = time2();
    diff = t1 - t2;
    printf("t1=%lld  t2=%lld  diff=%lld\n", t1, t2, diff);

    t2 = time2();
    uint64_t t3 = time3();
    printf("t2=%lld  t3=%lld\n", t2, t3);
    return 0;
}

