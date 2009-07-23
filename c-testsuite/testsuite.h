#ifndef TESTSUITE_H
#define TESTSUITE_H

#include <stdlib.h>

extern unsigned int failed;

struct testcase {	
	const char* name;
	void (*testcase)(void);
};

#define SUITE(a) const struct testcase a[]

#define ADD_TEST(a) { #a, a }
#define END_TEST {NULL, NULL}

void assert_equal(int expected, int real, const char* caller, int line);
#define ASSERT_EQUAL(a,b) assert_equal(a, b, __FILE__, __LINE__)

void assert_not_equal(int expected, int real, const char* caller, int line);
#define ASSERT_NOT_EQUAL(a,b) assert_not_equal(a, b, __FILE__, __LINE__)

void assert_not_null(void* real, const char* caller, int line);
#define ASSERT_NOT_NULL(a) assert_not_null(a, __FILE__, __LINE__)

void assert_null(void* real, const char* caller, int line);
#define ASSERT_NULL(a) assert_null(a, __FILE__, __LINE__)

void assert_true(int real, const char* caller, int line);
#define ASSERT_TRUE(a) assert_true(a, __FILE__, __LINE__)

void assert_false(int real, const char* caller, int line);
#define ASSERT_FALSE(a) assert_false(a, __FILE__, __LINE__)

void assert_str_equal(const char* expected, const char* real, const char* caller, int line);
#define ASSERT_STR_EQUAL(a,b) assert_str_equal(a, b, __FILE__, __LINE__)

void assert_str_not_equal(const char* expected, const char* real, const char* caller, int line);
#define ASSERT_STR_NOT_EQUAL(a,b) assert_str_not_equal(a, b, __FILE__, __LINE__)

void assert_fail(const char* caller, int line);
#define ASSERT_FAIL() assert_fail(__FILE__, __LINE__)

#endif

