#include "ringbuf_lf.h"

#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void ringbuf_lf_init(ringbuf_lf* rb, uint8_t* buf, uint32_t bufsize) {
    rb->buf = buf;
    rb->bufsize = bufsize;
    rb->head = 0;
    rb->tail = 0;
}

uint32_t ringbuf_lf_get_count(const ringbuf_lf* rb) {
    uint32_t head = rb->head;
    uint32_t tail = rb->tail;

    if (head == tail) return 0;
    if (head > tail) {
        return head - tail;
    }
    return rb->bufsize - (tail - head);
}

uint32_t ringbuf_lf_get_free(const ringbuf_lf* rb) {
    return rb->bufsize - ringbuf_lf_get_count(rb) - 1;  // always lose one
}

uint32_t ringbuf_lf_add(ringbuf_lf* rb, const uint8_t* data, uint32_t num) {
    uint32_t inserted = MIN(num, ringbuf_lf_get_free(rb));
    if (inserted == 0) return 0;

    uint32_t head = rb->head;
    uint32_t tail = rb->tail;
    if (head >= tail) {
        // 2 parts: head -> end, begin -> tail -1
        uint32_t part1 = MIN(inserted, rb->bufsize - head);
        memcpy(&rb->buf[head], data, part1);
        head = (head + part1) % rb->bufsize;
        uint32_t part2 = inserted - part1;
        if (part2) {
            memcpy(&rb->buf[0], &data[part1], part2);
            head = part2;
        }
    } else {
        // 1 part: head -> tail -1
        uint32_t part1 = inserted;
        memcpy(&rb->buf[head], data, part1);
        head = (head + part1) % rb->bufsize;
    }
    rb->head = head;
    return inserted;
}

uint32_t ringbuf_lf_get(ringbuf_lf* rb, uint8_t* data, uint32_t max)
{
    uint32_t head = rb->head;
    uint32_t tail = rb->tail;
    uint32_t count = MIN(ringbuf_lf_get_count(rb), max);
    if (count == 0) return 0;

    if (head > tail) {
        // 1 part: tail -> head -1
        memcpy(data, &rb->buf[tail], count);
        tail += count;
    } else {
        // 2 parts: tail -> end, begin -> head -1
        uint32_t part1 = MIN(count, rb->bufsize - tail);
        memcpy(data, &rb->buf[tail], part1);
        tail = (tail + part1) % rb->bufsize;
        uint32_t part2 = count - part1;
        if (part2) {
            memcpy(&data[part1], &rb->buf[0], part2);
            tail = part2;
        }
    }
    rb->tail = tail;
    return count;
}

