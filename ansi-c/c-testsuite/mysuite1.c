#include "testsuite.h"

void mytest1(void) {
    ASSERT_EQUAL(10, 2);    
}
void mytest2(void) {}


SUITE(mysuite1) = {
    ADD_TEST(mytest1),
    ADD_TEST(mytest2),
    END_TEST
};

