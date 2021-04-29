#include "ringbuf.h"
#include <string.h>

#define TAIL(x) ((rb->head + rb->count) % rb->bufsize)
#define MIN(a, b) ((a) < (b) ? (a) : (b))

void ringbuf_init(ringbuf_t* rb, uint8_t* buf, uint32_t bufsize) {
    rb->buf = buf;
    rb->bufsize = bufsize;
    rb->head = 0;
    rb->count = 0;
}

uint32_t ringbuf_add(ringbuf_t* rb, uint8_t data) {
    if (ringbuf_isfull(rb)) return 0;
    uint32_t tail = TAIL(rb);
    rb->buf[tail] = data;
    rb->count++;
    return 1;
}

unsigned ringbuf_add_mult(ringbuf_t* rb, const uint8_t* data, unsigned num) {
    if (ringbuf_isfull(rb)) return 0;

    uint32_t inserted = MIN(num, ringbuf_getfree(rb));
    uint32_t tail = TAIL(rb);

    uint32_t cnt1 = rb->bufsize - tail;
    if (inserted < cnt1) cnt1 = inserted;
    memcpy(&rb->buf[tail], data, cnt1);

    uint32_t cnt2 = inserted - cnt1;
    if (cnt2) memcpy(&rb->buf[0], &data[cnt1], cnt2);

    rb->count += inserted;
    return inserted;
}

unsigned ringbuf_get_mult(ringbuf_t* rb, uint8_t* data, unsigned max) {
    if (ringbuf_isempty(rb)) return 0;

    uint32_t retrieved = MIN(rb->count, max);

    uint32_t cnt1 = rb->bufsize - rb->head;
    if (max < cnt1) cnt1 = max;
    if (cnt1 > retrieved) cnt1 = retrieved;
    memcpy(data, &rb->buf[rb->head], cnt1);

    uint32_t cnt2 = retrieved - cnt1;
    if (cnt2) memcpy(&data[cnt1], &rb->buf[0], cnt2);

    rb->head = (rb->head + retrieved) % rb->bufsize;
    rb->count -= retrieved;
    return retrieved;
}

unsigned ringbuf_peek_tail(const ringbuf_t* rb, uint8_t* data, unsigned max) {
    if (ringbuf_isempty(rb)) return 0;

    uint32_t retrieved = MIN(rb->count, max);
    uint32_t head2 = (rb->head + rb->count - retrieved + rb->bufsize) % rb->bufsize;
    uint32_t cnt1 = rb->bufsize - head2;
    if (max < cnt1) cnt1 = max;
    if (cnt1 > retrieved) cnt1 = retrieved;
    memcpy(data, &rb->buf[head2], cnt1);

    uint32_t cnt2 = retrieved - cnt1;
    if (cnt2) memcpy(&data[cnt1], &rb->buf[0], cnt2);

    return retrieved;
}

