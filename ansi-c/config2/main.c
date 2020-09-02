#include <stdio.h>

#include "config.h"

int main(int argc, const char *argv[])
{
#ifdef FEATURE_1
    printf("Feature1 - enabled\n");
#else
    printf("Feature1 - disabled\n");
#endif
#ifdef FEATURE_2
    printf("Feature2 - enabled\n");
#else
    printf("Feature2 - disabled\n");
#endif
#ifdef FEATURE_3
    printf("Feature3 - enabled\n");
#else
    printf("Feature3 - disabled\n");
#endif
    return 0;
}

