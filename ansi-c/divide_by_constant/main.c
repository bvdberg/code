// Source - https://stackoverflow.com/a
// Posted by geza, modified by community. See post 'Timeline' for change history
// Retrieved 2025-11-18, License - CC BY-SA 3.0

#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint32_t mul;
    uint32_t add;
    int32_t shift;
} Divider;

static int32_t indexOfMostSignificantBit(uint32_t num) {
    for (int i = 31; i != 0; i--) {
        if (num >> i) return i;
    }
    return 0;
}

void set(Divider* div, uint32_t divider) {
    int32_t l = indexOfMostSignificantBit(divider);
    if (divider&(divider-1)) {
        uint64_t m = (uint64_t)(1)<<(l+32);
        div->mul = (uint32_t)(m/divider);

        uint32_t rem = (uint32_t)(m)-div->mul*divider;
        uint32_t e = divider-rem;

        if (e < (uint32_t)(1)<<l) {
            div->mul++;
            div->add = 0;
        } else {
            div->add = div->mul;
        }
        div->shift = l;
    } else {
        if (divider==1) {
            div->mul = 0xffffffff;
            div->add = 0xffffffff;
            div->shift = 0;
        } else {
            div->mul = 0x80000000;
            div->add = 0;
            div->shift = l-1;
        }
    }
}

static void test(uint32_t dvalue, uint32_t value) {
    Divider div = {};
    set(&div, dvalue);

    printf("dvalue %u  value %d   ", dvalue, value);
    printf("  mul %u", div.mul);
    printf("  add %u", div.add);
    printf("  sht %d", div.shift);

    // perform divide by multiply, add and shift
    uint32_t result = (uint32_t)(((uint64_t)(value)* div.mul + div.add) >> 32) >> div.shift;
    printf("  result %u\n", result);
}

int main(int argc, const char *argv[])
{
    for (uint32_t i = 0; i < 20; i++) {
        test(5, i);
    }

    test(13, 100);
    return 0;
}

