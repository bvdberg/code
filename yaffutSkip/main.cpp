#define YAFFUT_MAIN
#include "yaffut.h"

class A{
public:
    A() {
    printf("   ZZ %s\n", __func__);

    }
};

TEST(A, test1) {
    printf("   ZZ %s\n", __func__);
}

TEST_SKIP(A, test2) {
    printf("   ZZ %s\n", __func__);
}

