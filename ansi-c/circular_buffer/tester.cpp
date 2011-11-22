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


TEST(CircBufTest, testNumber) {
}


