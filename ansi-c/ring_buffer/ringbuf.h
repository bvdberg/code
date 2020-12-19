#ifndef RING_BUF_H
#define RING_BUF_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t* buf;
    uint32_t bufsize;   // must be multiple of two
    uint32_t head;      // read-index
    uint32_t count;
} ringbuf_t;

void ringbuf_init(ringbuf_t* rb, uint8_t* buf, uint32_t bufsize);

static inline void ringbuf_flush(ringbuf_t* rb) {
    rb->head = 0;
    rb->count = 0;
}

static inline uint32_t ringbuf_getsize(const ringbuf_t* rb) {
    return rb->count;
}

static inline uint32_t ringbuf_getfree(const ringbuf_t* rb) {
    return rb->bufsize - rb->count;
}

static inline bool ringbuf_isfull(const ringbuf_t* rb) {
    return rb->count == rb->bufsize;
}

static inline bool ringbuf_isempty(const ringbuf_t* rb) {
    return rb->count == 0;
}

//  return number of inserted items
unsigned ringbuf_add(ringbuf_t* rb, uint8_t data);

//  return number of inserted items
unsigned ringbuf_add_mult(ringbuf_t* rb, const uint8_t* data, unsigned num);

//  return number of retrieved items
unsigned ringbuf_get_mult(ringbuf_t* rb, uint8_t* data, unsigned max);

#endif

