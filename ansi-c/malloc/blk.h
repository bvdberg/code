#ifndef BLK_H
#define BLK_H

#include <stdint.h>

void blk_init();

// NOTE: malloc/free must be in 24-bytes multiples! (size of blk_t)
void* blk_malloc(uint32_t size);

void blk_free(void* ptr);

// ---- debug
void blk_dump();
uint32_t blk_get_buf();
#endif

