#include <stdio.h>
#include <assert.h>

static unsigned getMask(unsigned first, unsigned last) {
    unsigned mask = 0xffffffff;
    mask >>= (31-last);

    if (first != 0) {
        unsigned mask2 = 0xffffffff;
        mask2 >>= (32-first);
        mask -= mask2;
    }
    return mask;
}

static void printMask(unsigned first, unsigned last) {
    assert(first <= last);
    printf("Mask for [%2d:%2d] = 0x%04x\n", first, last, getMask(first, last));
}

int main(int argc, const char *argv[])
{
    printMask(0, 0);
    printMask(0, 4);
    printMask(2, 2);
    printMask(2, 4);
    printMask(3, 4);
    printMask(0, 15);

    return 0;
}

