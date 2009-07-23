#include <stdio.h>
#include <time.h>
#include <string.h>

#include <unistd.h>
#include <stdint.h>

__inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
}

const char* getTime() {
    static char buffer[10];
    uint64_t one = rdtsc();
    time_t now = time(0);
    struct tm* now2 = localtime(&now);
    //struct tm* now2 = gmtime(&now);
    sprintf(buffer, "%02d:%02d:%02d", now2->tm_hour, now2->tm_min, now2->tm_sec);
    uint64_t two = rdtsc();
    printf("diff = %llu (%lu usec)\n", two - one, (two-one) / 3000);
    return buffer;
}

int main() {
    time_t now = time(0);
    printf("ctime: %s", ctime(&now));
 
    
    struct tm* now2 = gmtime(&now);
    printf("gmtime: %02d:%02d:%02d\n", now2->tm_hour, now2->tm_min, now2->tm_sec);

    char buffer[20];
    strftime(buffer, 20, "%A %p", now2); 
    printf("strftime (variable): %s\n", buffer);

    printf("now = %s\n", getTime());
    return 0;
}

