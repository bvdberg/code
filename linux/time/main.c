#include <stdio.h>
#include <sys/time.h>
#include <time.h>



int main(int argc, const char *argv[])
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    //printf("TV: %lu\n", tv.tv_sec);

    time_t now;
    struct tm* tm;

    now = 0;
    tm = localtime(&now);
    printf("TM: [%10lu] %d-%d-%d %02d:%02d:%02d\n", now, tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    time(&now);
    tm = localtime(&now);
    printf("TM: [%10lu] %d-%d-%d %02d:%02d:%02d\n", now, tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    
    now = ((unsigned)(-1))/2;
    tm = localtime(&now);
    printf("TM: [%10lu] %d-%d-%d %02d:%02d:%02d\n", now, tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    
    now = (unsigned)(-1);
    tm = localtime(&now);
    printf("TM: [%10lu] %d-%d-%d %02d:%02d:%02d\n", now, tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    return 0;
}

