#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, const char *argv[])
{
    time_t now = time(0);
    fprintf(stdout, "writer: test output %ld\n", now);
    fflush(stdout);
    //fprintf(stderr, "writer: test error %ld", now);

    int i;
    for (i=0; i<100; i++) {
        printf("writer: output %d\n", i);
        fflush(stdout);
        sleep(1);
    }

    printf("writer: end\n");
    fflush(stdout);
    return 0;
}

