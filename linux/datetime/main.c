#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#if 0 
int tm_sec;         /* seconds */
int tm_min;         /* minutes */
int tm_hour;        /* hours */
int tm_mday;        /* day of the month */
int tm_mon;         /* month */
int tm_year;        /* year */
int tm_wday;        /* day of the week */
int tm_yday;        /* day in the year */
int tm_isdst;       /* daylight saving time */
#endif

const char* getTime() {
    static char buffer[40];
    time_t now = time(0);
    struct tm* now2 = localtime(&now);
    sprintf(buffer, "%d-%02d-%02d %02d:%02d:%02d",
        now2->tm_year + 1900, now2->tm_mon + 1, now2->tm_mday,
        now2->tm_hour, now2->tm_min, now2->tm_sec);
    return buffer;
}


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

    printf("custom = %s\n", getTime());
    return 0;
}

