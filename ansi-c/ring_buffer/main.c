#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ringbuf.h"
#include "color.h"

static void dump(const ringbuf_t* rb) {
    uint32_t sz = ringbuf_getsize(rb);
    printf("\n");
    printf("[");
    for (unsigned i=0; i<rb->bufsize; i++) {
        uint8_t c = rb->buf[i];
        printf("%c", isprint(c) ? c : '_');
    }
    printf("]\n");
    char tmp[128];
    memset(tmp, ' ', sizeof(tmp));
    tmp[127] = 0;
    tmp[rb->head + strlen("[")] = 'H';
    unsigned tail = (rb->head + rb->count) % rb->bufsize;
    tmp[tail + strlen("[")] = 'T';
    printf("%s\n", tmp);
}

static void add(ringbuf_t* rb, const char* str) {
    unsigned len = strlen(str);
    unsigned inserted;
    if (len == 1) {
        inserted = ringbuf_add(rb, str[0]);
    } else {
        inserted = ringbuf_add_mult(rb, (uint8_t*)str, len);
    }
    printf("%sinserting [%s] %u/%u%s\n", ANSI_GREEN, str, inserted, len, ANSI_NORMAL);
    dump(rb);
}

static void get(ringbuf_t* rb, unsigned count) {
    uint8_t out[128];
    unsigned numread = ringbuf_get_mult(rb, out, count);
    out[numread] = 0;
    printf("%sread [%s] %u/%u%s\n", ANSI_RED, out, numread, count, ANSI_NORMAL);
    dump(rb);
}

static void flush(ringbuf_t* rb) {
    ringbuf_flush(rb);
    memset(rb->buf, 0, rb->bufsize);
    dump(rb);
}

int main(int argc, const char *argv[])
{
    ringbuf_t rb;
    uint8_t buf[40];
    memset(buf, 0, sizeof(buf));
    ringbuf_init(&rb, buf, sizeof(buf));
    ringbuf_flush(&rb);

    dump(&rb);
    add(&rb, "a");
    ringbuf_add(&rb, 'b');
    add(&rb, "defghijk");
    add(&rb, "-1234567890-");
    add(&rb, "ABCDEFGHIJKLMNOPQ");
    get(&rb, 9);
    add(&rb, "1234");
    add(&rb, "567890123");
    get(&rb, 22);
    get(&rb, 15);
    flush(&rb);
    add(&rb, "1234567890acbdefghij");
    get(&rb, 20);
    add(&rb, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    get(&rb, 30);


    return 0;
}

