/*
 * Conclusion: setitimer makes weird jumps sometimes. Otherwise has a 1-3ms accuracy
 */
 
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>

u_int64_t getCurrentTime() {
    struct timeval now;
    gettimeofday(&now, 0);
    u_int64_t now64 = now.tv_sec * 1000000 + now.tv_usec;
    return now64;
}

volatile bool signal_received = false;

void handler(int) {
    signal_received = true;
}

int main() {
    struct itimerval value;
    struct itimerval ovalue;
     
    signal(SIGALRM, &handler);
   
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = 0;
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 100 * 1000;
    
    setitimer(ITIMER_REAL, &value, &ovalue);
    u_int64_t before = getCurrentTime();
    while(!signal_received) {}
    u_int64_t after = getCurrentTime();
    int32_t duration = after - before;
    printf("select duration = %d us.\n", duration);
      
    return 0;
}

