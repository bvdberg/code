#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

static u_int64_t total;
static u_int64_t count;

int main(int argc, const char *argv[])
{
    count = argc - 1;
    unsigned int i;
    for (i=1; i<=count; i++) {
        total += atoi(argv[i]);
    }
    double total_d = total;
    printf("total %llu  total=%llu  avg=%f\n", count, total, total_d / count);
    return 0;
}

