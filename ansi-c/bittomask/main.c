#include <stdio.h>
#include <stdint.h>
#include <assert.h>

static uint32_t getMask32(uint32_t first, uint32_t last) {
    assert(first <= last);
    uint32_t mask = (uint32_t)-1;
    mask >>= (31-last);

    if (first != 0) {
        uint32_t mask2 = (uint32_t)-1;
        mask2 >>= (32-first);
        mask -= mask2;
    }
    return mask;
}

static uint64_t getMask64(uint64_t first, uint64_t last) {
    assert(first <= last);
    uint64_t mask = (uint64_t)-1;
    mask >>= (63-last);

    if (first != 0) {
        uint64_t mask2 = (uint64_t)-1;
        mask2 >>= (64-first);
        mask -= mask2;
    }
    return mask;
}

static void printMask32(uint32_t first, uint32_t last) {
    printf("Mask32 for [%2d:%2d] = 0x%04x\n", first, last, getMask32(first, last));
}

static void printMask64(uint64_t first, uint64_t last) {
    printf("Mask64 for [%2d:%2d] = 0x%04llx\n", (int)first, (int)last, getMask64(first, last));
}

int main(int argc, const char *argv[])
{
    printMask32(0, 0);
    printMask32(0, 4);
    printMask32(2, 2);
    printMask32(2, 4);
    printMask32(3, 4);
    printMask32(0, 15);
    printf("\n");
    printMask64(0, 0);
    printMask64(6, 7);
    printMask64(31, 31);
    printMask64(63, 63);


    return 0;
}

