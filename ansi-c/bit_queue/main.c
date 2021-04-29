#include <stdio.h>
#include <stdint.h>
#include "color.h"

#define BIT(x) (1 << x)

static uint32_t pending;
static uint8_t head;

static uint32_t busy;

static void set(uint8_t bit) {
    printf("%sstart %u%s\n", ANSI_GREEN, bit, ANSI_NORMAL);
    pending |= BIT(bit);
    busy |= BIT(bit);
}

static void done(uint8_t bit) {
    printf("%sfinish %u%s\n", ANSI_GREEN, bit, ANSI_NORMAL);
    busy &= ~BIT(bit);
}


static void dump(void) {
    printf("pending %08X  head %u\n", pending, head);
    printf("   busy %08X\n", busy);
}

static void check(void) {
    printf("check  [H %u] %08X  %08X\n", head, pending, busy);
    if (pending == 0) return;

    while (pending) {
        if (pending |= BIT(head)) {
            if ((busy & BIT(head)) != 0) return;
            printf("%s-> free %u%s\n", ANSI_BLUE, head, ANSI_NORMAL);
            pending &= ~BIT(head);
        }
        head++;
    }
}

int main(int argc, const char *argv[])
{
    dump();
    check();
    set(0);
    check();
    set(1);
    set(2);
    dump();
    check();
    done(0);
    check();
    done(2);
    check();
    done(1);
    check();
    dump();


    
    return 0;
}

