#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

const char* getTime() {
    static char buffer[10];
    time_t now = time(0);
    struct tm* now2 = localtime(&now);
    //struct tm* now2 = gmtime(&now);
    sprintf(buffer, "%02d:%02d:%02d", now2->tm_hour, now2->tm_min, now2->tm_sec);
    return buffer;
}


void time1() {
    struct timeval now;
    gettimeofday(&now, NULL);
    printf("gettimeofday  = %ld.%03ld\n", now.tv_sec, now.tv_usec/1000);
}
/*
void time2() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    //clock_gettime(CLOCK_REALTIME, &now);
    printf("clock_gettime = %ld.%03ld\n", now.tv_sec, now.tv_nsec/1000000);
}
*/

int main() {
    time_t now = time(0);
    printf("time(0) = %ld\n", now);

    struct timeval t1;
    gettimeofday(&t1, NULL);
    printf("gettimeofday = %ld\n", t1.tv_sec);

    printf("ctime: %s", ctime(&now));
 
    
    struct tm* now2 = gmtime(&now);
    printf("gmtime: %02d:%02d:%02d\n", now2->tm_hour, now2->tm_min, now2->tm_sec);

    char buffer[20];
    strftime(buffer, 20, "%A %p", now2); 
    printf("strftime (variable): %s\n", buffer);

    printf("now = %s\n", getTime());
    return 0;
}

