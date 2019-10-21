#include <stdio.h>
#include <unistd.h>

int online_cpus()
{
    long ncpus = sysconf(_SC_NPROCESSORS_ONLN);
    if (ncpus > 0) return (int)ncpus;
    return 1;
}

int main(int argc, const char *argv[])
{
    printf("detected %d cpus\n", online_cpus());
    return 0;
}

