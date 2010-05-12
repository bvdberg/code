#include <stdio.h>

static const unsigned char table[] =
{
#include "file.h"
};

int main(int argc, const char *argv[])
{
    int size = sizeof(table);
    printf("size=%d\n", size);
    int i;
    for (i=0; i<size; i++) {
        printf("%c", table[i]);
    }
    return 0;
}

