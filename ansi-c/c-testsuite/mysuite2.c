#include "testsuite.h"

void mytest1_2(void) {}
void mytest2_2(void) {}

SUITE(mysuite2) = {
    ADD_TEST(mytest1_2),
    ADD_TEST(mytest2_2),
    END_TEST
};

