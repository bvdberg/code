#include <stdint.h>
#include <string.h>

#include "ctest.h"
#include "ringbuf_lf.h"

#define BUF_SIZE 8

static const uint8_t example1[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
static const uint8_t example2[] = { 9, 10, 11, 12 };

static void dump(const ringbuf_lf* rb) {
    CTEST_LOG("H %u  T %u  S %u", (uint32_t)rb->head, (uint32_t)rb->tail, (uint32_t)rb->bufsize);
}

CTEST_DATA(rb) {
    ringbuf_lf ring;
    uint8_t buf[BUF_SIZE];
};

CTEST_SETUP(rb) {
    ringbuf_lf_init(&data->ring, data->buf, BUF_SIZE);
}

CTEST2(rb, empty) {
    ASSERT_EQUAL(BUF_SIZE-1, ringbuf_lf_get_free(&data->ring));
    ASSERT_TRUE(ringbuf_lf_isempty(&data->ring));
    uint8_t tmp[20];
    ASSERT_EQUAL(0, ringbuf_lf_get(&data->ring, tmp, sizeof(tmp)));
}

CTEST2(rb, add_get) {
    ASSERT_EQUAL(5, ringbuf_lf_add(&data->ring, example1, 5));
    ASSERT_EQUAL(2, ringbuf_lf_get_free(&data->ring));
    ASSERT_EQUAL(5, ringbuf_lf_get_count(&data->ring));
    ASSERT_FALSE(ringbuf_lf_isempty(&data->ring));

    uint8_t tmp[BUF_SIZE] = { 0 };
    ASSERT_EQUAL(5, ringbuf_lf_get(&data->ring, tmp, sizeof(tmp)));
    ASSERT_DATA(example1, 5, tmp, 5);
    ASSERT_EQUAL(7, ringbuf_lf_get_free(&data->ring));
    ASSERT_EQUAL(0, ringbuf_lf_get_count(&data->ring));
    ASSERT_TRUE(ringbuf_lf_isempty(&data->ring));
}

CTEST2(rb, add_wrapping) {
    data->ring.head = 4;
    data->ring.tail = 4;

    ASSERT_EQUAL(5, ringbuf_lf_add(&data->ring, example1, 5));
    ASSERT_EQUAL(2, ringbuf_lf_get_free(&data->ring));
    ASSERT_EQUAL(5, ringbuf_lf_get_count(&data->ring));
    ASSERT_FALSE(ringbuf_lf_isempty(&data->ring));

    uint8_t tmp[BUF_SIZE] = { 0 };
    ASSERT_EQUAL(5, ringbuf_lf_get(&data->ring, tmp, sizeof(tmp)));
    ASSERT_DATA(example1, 5, tmp, 5);
    //dump(&data->ring);
    ASSERT_EQUAL(7, ringbuf_lf_get_free(&data->ring));
    ASSERT_EQUAL(0, ringbuf_lf_get_count(&data->ring));
    ASSERT_TRUE(ringbuf_lf_isempty(&data->ring));
}

CTEST2(rb, add_all) {
    ASSERT_EQUAL(7, ringbuf_lf_add(&data->ring, example1, 8));
    ASSERT_EQUAL(0, ringbuf_lf_get_free(&data->ring));
    ASSERT_EQUAL(7, ringbuf_lf_get_count(&data->ring));
    ASSERT_FALSE(ringbuf_lf_isempty(&data->ring));

    ASSERT_EQUAL(0, ringbuf_lf_add(&data->ring, example2, 1));
}

CTEST2(rb, add_multiple) {
    ASSERT_EQUAL(4, ringbuf_lf_add(&data->ring, example1, 4));
    ASSERT_EQUAL(3, ringbuf_lf_get_free(&data->ring));
    ASSERT_EQUAL(4, ringbuf_lf_get_count(&data->ring));

    ASSERT_EQUAL(2, ringbuf_lf_add(&data->ring, &example1[4], 2));
    ASSERT_EQUAL(1, ringbuf_lf_get_free(&data->ring));
    ASSERT_EQUAL(6, ringbuf_lf_get_count(&data->ring));

    ASSERT_EQUAL(1, ringbuf_lf_add(&data->ring, example2, 3));
    ASSERT_EQUAL(0, ringbuf_lf_get_free(&data->ring));
    ASSERT_EQUAL(7, ringbuf_lf_get_count(&data->ring));
}

CTEST2(rb, add_more_wrapped) {
    data->ring.head = 2;
    data->ring.tail = 6;
    const uint8_t content[BUF_SIZE] = { 3, 4, 0, 0, 0, 0, 1, 2 };
    memcpy(data->ring.buf, content, BUF_SIZE);
    ASSERT_EQUAL(4, ringbuf_lf_get_count(&data->ring));
    ASSERT_EQUAL(3, ringbuf_lf_get_free(&data->ring));

    ASSERT_EQUAL(3, ringbuf_lf_add(&data->ring, example2, 4));
    //dump(&data->ring);
    ASSERT_EQUAL(0, ringbuf_lf_get_free(&data->ring));
    ASSERT_EQUAL(7, ringbuf_lf_get_count(&data->ring));

    uint8_t tmp[BUF_SIZE] = { 0 };
    ASSERT_EQUAL(7, ringbuf_lf_get(&data->ring, tmp, sizeof(tmp)));
    const uint8_t exp[] = { 1, 2, 3, 4, 9, 10, 11 };
    ASSERT_DATA(exp, sizeof(exp), tmp, 7);
    ASSERT_EQUAL(7, ringbuf_lf_get_free(&data->ring));
    ASSERT_EQUAL(0, ringbuf_lf_get_count(&data->ring));
    ASSERT_TRUE(ringbuf_lf_isempty(&data->ring));
}

