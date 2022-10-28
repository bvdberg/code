#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


#include "data.h"
/*
 * To test the 32-bit speedup in relation to 64-bit systems,
 * put X pointers in an array and search them all for dups. This
 * simulates some other data-structures somewhat
 *
 * Linux is weird: adding the print("SIZE=..") in data.c makes the 32-bit program
 * run twice as fast!?!? (1.2s insteaf of 2.4s)
 */

#define Max 200000

static u_int64_t getCurrentTime()
{
    struct timeval now;
    gettimeofday(&now, 0);
    u_int64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += (now.tv_usec);
    return now64;
}


int main(int argc, const char *argv[])
{
    printf("Sizeof(void*) %u\n", (uint32_t)sizeof(void*));

    data_init(Max);

    uint64_t t1 = getCurrentTime();
    for (uintptr_t i=0; i<Max; i++) {
        data_add((void*)i);
    }
    uint64_t t2 = getCurrentTime();
    printf("took %" PRIu64 " usec\n", t2 - t1);

    data_free();
    return 0;
}

