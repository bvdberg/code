#include <stdio.h>
#include <sys/types.h>
#include <utime.h>


int main(int argc, const char *argv[])
{
    if (argc != 2) {
        printf("Usage: test [file]\n");
        return -1;
    }

    int ok = utime(argv[1], NULL);
    if (ok == -1) {
        perror("utime");
        return -1;
    }

    return 0;
}

