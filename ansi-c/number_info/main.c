#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

static u_int64_t total;
static u_int64_t count;
static int min = 2000000000;
static int max;

int main(int argc, const char *argv[])
{
    count = argc - 1;
    unsigned int i;
    for (i=1; i<=count; i++) {
        int num = atoi(argv[i]);
        total += num;
        if (num < min) min = num;
        if (num > max) max = num;
    }
    double total_d = total;
    printf("%llu numbers:  min=%d  max=%d  sum=%llu  avg=%f\n", count, min, max, total, total_d / count);
    return 0;
}

