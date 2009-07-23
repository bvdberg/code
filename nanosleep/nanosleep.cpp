/*
 * Conclusion: nanosleep 15+ ms delays are 2-3ms too late.
 */
 
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>

u_int64_t getCurrentTime() {
    struct timeval now;
    gettimeofday(&now, 0);
    u_int64_t now64 = now.tv_sec * 1000000 + now.tv_usec;
    return now64;
}


int main() {
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = 100 * 1000 * 1000;  // 100 ms
    
    u_int64_t before = getCurrentTime();
    nanosleep(&req, 0);
    u_int64_t after = getCurrentTime();
    int32_t duration = after - before;
    printf("nanosleep duration = %d us.\n", duration);
      
    return 0;
}

