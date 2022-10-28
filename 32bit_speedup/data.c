#include "data.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * To test the 32-bit speedup in relation to 64-bit systems,
 * put X pointers in an array and search them all for dups. This
 * simulates some other data-structures somewhat
 */

typedef struct {
    void** list;
    uint32_t count;
} Data;

static Data data;

void data_init(uint64_t max) {
    data.list = malloc(max * sizeof(void*));
    printf("SIZE %u\n", (uint32_t)max*sizeof(void*));
}

void data_add(void* d) {
    for (uint32_t i=0; i<data.count; i++) {
        if (data.list[i] == d) return;
    }
    data.list[data.count] = d;
    data.count++;
}

void data_free(void) {
    free(data.list);
}

