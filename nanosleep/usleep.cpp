/*
 * Conclusion: usleep 20+ ms delays are 2-4ms too late.
 * delays of 10 ms become 14-19ms
 */
 
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

u_int64_t getCurrentTime() {
    struct timeval now;
    gettimeofday(&now, 0);
    u_int64_t now64 = now.tv_sec * 1000000 + now.tv_usec;
    return now64;
}


int main() {
    u_int64_t before = getCurrentTime();
    usleep(10 * 1000);
    u_int64_t after = getCurrentTime();
    int32_t duration = after - before;
    printf("usleep duration = %d us.\n", duration);
      
    return 0;
}

