/*
 * Copyright INBRAIN-Neuroelectronics 2021-2022
 * All rights reserved
 */

#ifndef RING_BUF_LF_H
#define RING_BUF_LF_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t head;      // write-index
    uint32_t tail;      // read-index
    uint32_t bufsize;   // must be multiple of two
    uint8_t* buf;
} ringbuf_lf;

void ringbuf_lf_init(ringbuf_lf* rb, uint8_t* buf, uint32_t bufsize);

uint32_t ringbuf_lf_get_free(const ringbuf_lf* rb);

uint32_t ringbuf_lf_get_count(const ringbuf_lf* rb);

static inline bool ringbuf_lf_isempty(const ringbuf_lf* rb) {
    return rb->head == rb->tail;
}

// return number of inserted items
// only modifies head
uint32_t ringbuf_lf_add(ringbuf_lf* rb, const uint8_t* data, uint32_t num);

// return number of removed items
// only modifies tail
uint32_t ringbuf_lf_get(ringbuf_lf* rb, uint8_t* data, uint32_t max);

#endif

