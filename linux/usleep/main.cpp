#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{

    timeval startTime;
    timeval endTime;
    gettimeofday(&startTime, 0);

    for(int i=0; i<1000; i++)
    {
        usleep(atoi(argv[1]));
    }
    
    gettimeofday(&endTime, 0);
    
    printf("1000 x usleep(%d)\n", atoi(argv[1]));

    u_int64_t start64 = startTime.tv_sec * (u_int64_t)1000000 + startTime.tv_usec;
    u_int64_t end64 = endTime.tv_sec * (u_int64_t)1000000 + endTime.tv_usec;
    int diff = (int)(end64 - start64);
    printf("startTime = %06d.%06d\n", startTime.tv_sec, startTime.tv_usec);
    printf("  endTime = %06d.%06d\n", endTime.tv_sec, endTime.tv_usec);
    printf("diff = %d ms\n", diff/1000);
        

    return 0;
}

