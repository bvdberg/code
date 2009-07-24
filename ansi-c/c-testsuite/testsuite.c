#include <stdio.h>
#include <string.h>

#include "testsuite.h"

unsigned int total_failed = 0;
unsigned int total_succeeded = 0;
unsigned int total_tests = 0;
unsigned int failed;


void 
assert_equal(int expected, int real, const char* caller, int line) {
    if (real == expected) return;
    printf("%s:%d  FAILED: expected %d, got %d\n", caller, line, expected, real);
    failed++;
}


void 
assert_not_equal(int expected, int real, const char* caller, int line) {
    if (real != expected) return;
    printf("%s:%d  FAILED: expected %d, got %d\n", caller, line, expected, real);
    failed++;
}


void
assert_not_null(void* real, const char* caller, int line) {
    if (real != NULL) return;
    printf("%s:%d  FAILED: should not be NULL\n", caller, line);
    failed++;
}


void
assert_null(void* real, const char* caller, int line) {
    if (real == NULL) return;
    printf("%s:%d  FAILED: should be NULL\n", caller, line);
    failed++;
}


void
assert_true(int real, const char* caller, int line) {
    if (real) return;
    printf("%s:%d  FAILED: should be true\n", caller, line);
    failed++;
}


void
assert_false(int real, const char* caller, int line) {
    if (!real) return;
    printf("%s:%d  FAILED: should be false\n", caller, line);
    failed++;
}


void
assert_str_equal(const char* expected, const char* real, const char* caller, int line) {
    if (strcmp(real, expected) == 0) return;
    printf("%s:%d  FAILED: expected '%s', got '%s'\n", caller, line, expected, real);
    failed++;
}


void
assert_str_not_equal(const char* expected, const char* real, const char* caller, int line) {
    if (strcmp(real, expected) != 0) return;
    printf("%s:%d  FAILED: should not get '%s'\n", caller, line, real);
    failed++;
}


void
assert_fail(const char* caller, int line) {
    printf("%s:%d  FAILED: should not come here\n", caller, line);
    failed++;
}


void runSuite(struct testcase* suite) {
	while (suite->name != NULL) {
		printf("%s:", suite->name);
        failed = 0;
        total_tests++;
		suite->testcase();
        if (failed) {
            total_failed++;
             printf(" FAILED\n");
        } else {
            total_succeeded++;
            printf(" PASS\n");
        }
        suite++;
	}
}


extern struct testcase mysuite1[];
extern struct testcase mysuite2[];


int main(void) { 
    runSuite(mysuite1);
    runSuite(mysuite2);

	printf("Tests run (%d):  %d passed  %d failed\n", total_tests, total_succeeded, total_failed);

    return total_failed;
}

