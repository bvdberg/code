/**
 * compile with:
  * gcc -lrt -o periodic nano-sleep.c
   **/

#include <time.h>
#include <stdio.h>

#define NSEC_PER_SEC    1000000000L

static inline void timespec_add_ns(struct timespec *a, unsigned int ns)
{
    ns += a->tv_nsec;
    while(ns >= NSEC_PER_SEC) {
        ns -= NSEC_PER_SEC;
        a->tv_sec++;
    }
    a->tv_nsec = ns;
}


static inline int timespec_compare(const struct timespec *lhs, const struct timespec *rhs)
{
    if (lhs->tv_sec < rhs->tv_sec) return -1;
    if (lhs->tv_sec > rhs->tv_sec) return 1;
    return lhs->tv_nsec - rhs->tv_nsec;
}


int main()
{
    struct timespec time, now;
    unsigned int interval = 100000000;

    clock_gettime(CLOCK_MONOTONIC, &time);
    while (1) {
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &time, 0);

        // Do the periodic work here.
        printf("Hello World\n");

        timespec_add_ns(&time, interval);

        // Drop cycles if we are too slow.
        // This is a failsave that might not be necessary or desired.
        clock_gettime(CLOCK_MONOTONIC, &now);
        if (timespec_compare(&time, &now) < 0) {
            time = now;
            timespec_add_ns(&time, interval);
            printf("Too Slow.\n");
        }
    }
    return 0;
}

