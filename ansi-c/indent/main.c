#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [indent]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int indent = atoi(argv[1]);

    printf("%*sBLABLA\n", 2 * indent, "");    // indent
    return 0;
}

