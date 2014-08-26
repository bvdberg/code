#include <stdio.h>
#include <assert.h>

static void printMask(unsigned first, unsigned last) {
    assert(first <= last);
    unsigned mask = 0;
    for (unsigned i=first; i<=last; i++) {
        mask |= (1<<i);
    }
    printf("Mask for [%d:%d] = 0x%04x\n", first, last, mask);
}

int main(int argc, const char *argv[])
{
    printMask(0, 0);
    printMask(0, 4);
    printMask(2, 2);
    printMask(2, 4);
    printMask(0, 15);

    return 0;
}

