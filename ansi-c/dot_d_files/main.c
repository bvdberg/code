#include <stdio.h>
#include "util.h"

// NOTE: this does NOT show up in the .d file
extern int number;

int main(int argc, const char *argv[])
{
    printf("number=%d\n", number);
    return 0;
}
