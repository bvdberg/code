#include <stdio.h>
#define YAFFUT_MAIN
#include "yaffut.h"
#include "myassert.h"

extern "C" {
#include "circbuf.h"
}

using namespace Hex;
using namespace std;

struct CircBufTest {
    Buffer buf;    
    CircBufTest() {
        buffer_init(&buf);
    }
};


TEST(CircBufTest, testEmpty) {
    ASSERT_EQUAL(0, buffer_data(&buf));
    ASSERT_EQUAL(BUF_SIZE-1, buffer_free(&buf));
}


TEST(CircBufTest, testAddingOneItem) {
    buffer_add(&buf, 1);
    ASSERT_EQUAL(1, buffer_data(&buf));
    ASSERT_EQUAL(BUF_SIZE-2, buffer_free(&buf));
}


TEST(CircBufTest, testAddingAndConsumingOne) {
    for (int i=0; i<BUF_SIZE+4; i++) {
        buffer_add(&buf, 1);
        buffer_remove(&buf, 1);
        ASSERT_EQUAL(0, buffer_data(&buf));
        ASSERT_EQUAL(BUF_SIZE-1, buffer_free(&buf));
    }
}


TEST(CircBufTest, testAddingAndConsumingOneOffsetOne) {
    buffer_add(&buf, 1);
    for (int i=0; i<BUF_SIZE+4; i++) {
        buffer_add(&buf, 1);
        buffer_remove(&buf, 1);
        ASSERT_EQUAL(1, buffer_data(&buf));
        ASSERT_EQUAL(BUF_SIZE-2, buffer_free(&buf));
    }
}
