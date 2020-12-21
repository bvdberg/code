#include "blk.h"
#include "utils/list.h"
#include <stdio.h>

#define BLK_BUF_SIZE 2048

#define BSIZE(x) (x & ~1)
#define BUSED(x) (x & 1)

typedef struct {
    uint32_t size;   // bit(0) = used
    list_tag list;
    uint8_t data[0];
} blk_t;

static uint8_t blk_buf[BLK_BUF_SIZE];
static list_tag blocks;

static void blk_create(blk_t* b, uint32_t size) {
    b->size = size - sizeof(blk_t);
}

void blk_init() {
    printf("SIZEOF=%lu\n", sizeof(blk_t));
    blk_t* b = (blk_t*)blk_buf;
    blk_create(b, BLK_BUF_SIZE);
    list_init(&blocks);
    list_add_tail(&blocks, &b->list);
}

void* blk_malloc(uint32_t size) {
    list_tag* node = blocks.next;
    while (node != &blocks) {
        blk_t* b = to_container(node, blk_t, list);
        // NOTE: we take the first fit, not the smallest!
        uint32_t bsize = BSIZE(b->size);
        if (!BUSED(b->size) && bsize >= size) {
            if (bsize > size) {
                if (bsize - size <= sizeof(blk_t)) {
                    size = bsize;
                } else {
                    blk_t* b2 = (blk_t*)&b->data[size];
                    blk_create(b2, bsize - size);
                    list_add_front(&b->list, &b2->list);
                }
            }
            b->size = size | 1;
            return b->data;
        }
        node = node->next;
    }
    return NULL;
}

void blk_free(void* ptr) {
    blk_t* b = to_container(ptr, blk_t, data);
    b->size &= ~1;
    if (b->list.next != &blocks) { // last block
        blk_t* next = to_container(b->list.next, blk_t, list);
        if (!BUSED(next->size)) { //  merge next into this
            b->size += (next->size + sizeof(blk_t));
            list_remove(&next->list);
        }
    }
    if (b->list.prev != &blocks) {  // first block
        blk_t* prev = to_container(b->list.prev, blk_t, list);
        if (!BUSED(prev->size)) { // merge this into next
            prev->size += (b->size + sizeof(blk_t));
            list_remove(&b->list);
        }
    }
}



void blk_dump() {
    printf("blocks:\n");
    list_tag* node = blocks.next;
    while (node != &blocks) {
        const blk_t* b = to_container(node, blk_t, list);
        uint32_t offset = ((uint64_t)(b))-(uint64_t)blk_buf;
        uint32_t offset2 = ((uint64_t)(b->data))-(uint64_t)blk_buf;
        printf("  %4u  %4u  %4u  %4u  %u\n", offset, offset2, BSIZE(b->size), offset2+BSIZE(b->size), BUSED(b->size));
        node = node->next;
    }
}

uint32_t blk_get_buf() {
    return (uint64_t)blk_buf;
}

