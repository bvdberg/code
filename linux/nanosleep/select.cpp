/*
 * Conclusion: select 15+ ms delays are between 0.5 ms early and late
 * 10 ms delays become 14-25ms
 *  5 ms delays are often correct, but can be 20ms
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
    struct timeval delay;
    delay.tv_sec = 0;
    delay.tv_usec = 10 * 1000;
    
    u_int64_t before = getCurrentTime();
    select(0, 0, 0, 0, &delay);
    u_int64_t after = getCurrentTime();
    int32_t duration = after - before;
    printf("select duration = %d us.\n", duration);
      
    return 0;
}

