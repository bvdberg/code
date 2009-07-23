#include <stdio.h>

struct test_t {
    char* name;
    void (*function)(void);
};

struct test_t test_begin __attribute__((unused,section(".testuite"))) = { 0, 0 };
struct test_t test_end __attribute__((unused,section(".testuite_end"))) = { 0, 0 };

#define TEST(a) \
void a(void); \
struct test_t testname__##a __attribute__((unused,section(".testuite"))) = { #a, a }; \
void a()

TEST(test1) {
    printf("test1\n");
}

TEST(test2) {
    printf("test2\n");
}


TEST(test3) {
    printf("test3\n");
}

TEST(test4) {
    printf("test4\n");
}

int main() {
    struct test_t* test;
    for (test = &test_begin+1; test != &test_end; test++) {
        printf("running [%s]\n", test->name);
        test->function();
    }

    return 0;
}


