#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, const char *argv[])
{
    time_t now = time(0);
    fprintf(stdout, "application: test output %ld\n", now);
    fprintf(stderr, "application: test error %ld\n", now);
/*
    int i;
    for (i=0; i<argc; i++) printf("arg[%d] = '%s'\n", i, argv[i]);
*/
    sleep(4);
#ifdef CRASH
    printf("crash\n");
    abort();  // to cause crash
#endif
    printf("end\n");
    return 0;
}

