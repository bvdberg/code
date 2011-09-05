#include <stdio.h>

static int values[16] =
{
    0x00000000,
    0x30fbc54c,
    0x5a827999,
    0x7641af3b,
    0x7fffffff,
    0x7641af3b,
    0x5a827999,
    0x30fbc54c,
    0x00000000,
    0xcf043ab4,
    0xa57d8667,
    0x89be50c5,
    0x80000001,
    0x89be50c5,
    0xa57d8667,
    0xcf043ab4,
};

int main(int argc, const char *argv[])
{
    unsigned int max = 0xffffffff;
    printf("max = 0x%08x  %u  %d\n", max, max, max);

    unsigned int maxs = 0x7fffffff;
    printf("maxs= 0x%08x  %u  %d\n", maxs, maxs, maxs);

    unsigned int mins = 0x80000000;
    printf("mins= 0x%08x  %u  %d\n", mins, mins, mins);

    unsigned int test = 0x8fffffff;
    printf("test= 0x%08x  %u  %d\n", test, test, test);
    int i;
    for (i=0; i<16; i++) {
         printf("%12d\n", values[i]);
    }
    return 0;
}
