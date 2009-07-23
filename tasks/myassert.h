#ifndef MYASSERT_H
#define MYASSERT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace Mud {

class Assert {
public:
static void 
assert_equal(int expected, int real, const char* caller, int line) {
    if (real == expected) return;
    printf("%s:%d  FAILED: expected %d, got %d\n", caller, line, expected, real);
    exit(-1);
}
static void 
assert_equal(void* expected, void* real, const char* caller, int line) {
    if (real == expected) return;
    printf("%s:%d  FAILED: expected %p, got %p\n", caller, line, expected, real);
    exit(-1);
}
#define ASSERT_EQUAL(a,b) Assert::assert_equal(a, b, __FILE__, __LINE__)


static void 
assert_not_equal(int expected, int real, const char* caller, int line) {
    if (real != expected) return;
    printf("%s:%d  FAILED: expected %d, got %d\n", caller, line, expected, real);
    exit(-1);
}
#define ASSERT_NOT_EQUAL(a,b) Assert::assert_not_equal(a, b, __FILE__, __LINE__)


static void
assert_not_null(void* real, const char* caller, int line) {
    if (real != NULL) return;
    printf("%s:%d  FAILED: should not be NULL\n", caller, line);
    exit(-1);
}
#define ASSERT_NOT_NULL(a)  Assert::assert_not_null(a, __FILE__, __LINE__)


static void
assert_null(void* real, const char* caller, int line) {
    if (real == NULL) return;
    printf("%s:%d  FAILED: should be NULL\n", caller, line);
    exit(-1);
}
#define ASSERT_NULL(a)  Assert::assert_null(a, __FILE__, __LINE__)


static void
assert_true(bool real, const char* caller, int line) {
    if (real) return;
    printf("%s:%d  FAILED: should be true\n", caller, line);
    exit(-1);
}
#define ASSERT_TRUE(a)  Assert::assert_true(a, __FILE__, __LINE__)


static void
assert_false(bool real, const char* caller, int line) {
    if (!real) return;
    printf("%s:%d  FAILED: should be false\n", caller, line);
    exit(-1);
}
#define ASSERT_FALSE(a)  Assert::assert_false(a, __FILE__, __LINE__)


static void
assert_str_equal(const char* expected, const char* real, const char* caller, int line) {
    if (strcmp(real, expected) == 0) return;
    printf("%s:%d  FAILED: expected '%s', got '%s'\n", caller, line, expected, real);
    exit(-1);
}
static void
assert_str_equal(const char* expected, const std::string& real, const char* caller, int line) {
    assert_str_equal(expected, real.c_str(), caller, line);
}
static void
assert_str_equal(const std::string& expected, const std::string& real, const char* caller, int line) {
    assert_str_equal(expected.c_str(), real.c_str(), caller, line);
}
#define ASSERT_STR_EQUAL(a,b)  Assert::assert_str_equal(a, b, __FILE__, __LINE__)


static void
assert_str_not_equal(const char* expected, const char* real, const char* caller, int line) {
    if (strcmp(real, expected) != 0) return;
    printf("%s:%d  FAILED: should not get '%s'\n", caller, line, real);
    exit(-1);
}
static void
assert_str_not_equal(const char* expected, const std::string& real, const char* caller, int line) {
    assert_str_not_equal(expected, real.c_str(), caller, line);
}
#define ASSERT_STR_NOT_EQUAL(a,b)  Assert::assert_str_not_equal(a, b, __FILE__, __LINE__)


static void
assert_fail(const char* caller, int line) {
    printf("%s:%d  FAILED: should not come here\n", caller, line);
    exit(-1);
}
#define ASSERT_FAIL()  Assert::assert_fail(__FILE__, __LINE__)

};

}

#endif

