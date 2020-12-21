#include <stdio.h>
#include <stdint.h>
#include "blk.h"

#define ANSI_RED      "\033[0;31m"
#define ANSI_GREEN    "\033[0;32m"
#define ANSI_YELLOW   "\033[0;33m"
#define ANSI_BLUE     "\033[0;34m"
#define ANSI_MAGENTA  "\033[0;35m"
#define ANSI_WHITE    "\033[01;37m"
#define ANSI_NORMAL   "\033[0m"

static uint32_t base;

static void* get(uint32_t size) {
    void* p = blk_malloc(size);
    if (p == NULL) printf(ANSI_YELLOW "alloc %4u -> NULL" ANSI_NORMAL "\n", size);
    else printf(ANSI_YELLOW "alloc %4u -> %u" ANSI_NORMAL "\n", size, (uint32_t)(((uint64_t)p)-base));
    blk_dump();
    return p;
}

static void put(void* p) {
    if (p == NULL) {
        printf("CANNOT FREE NULL\n");
        return;
    }
    printf(ANSI_YELLOW "free %u" ANSI_NORMAL "\n", (uint32_t)(((uint64_t)p)-base));
    blk_free(p);
    blk_dump();
}

int main(int argc, const char *argv[])
{
    blk_init();
    base = blk_get_buf();

    blk_dump();
    void* p1 = get(512);
    void* p2 = get(512);
    void* p3 = get(256);
    void* p4 = get(512);

    put(p1);
    put(p3);
    void* p5 = get(136);

    put(p4);
    put(p2);

    return 0;
}

