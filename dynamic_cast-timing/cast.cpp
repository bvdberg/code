#include <stdio.h>
#include <stdint.h>

/*
    Conclusion: dynamic_cast is pretty fast
*/


__inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
}

class Parent {
public:
    virtual void doIt() {
        printf("Parent\n");
    }
};

class Parent2 {
public:
    virtual void doIt2() {
        printf("Parent2\n");
    }
};

class Child1 : public Parent {
public:
    virtual void doIt() {
        printf("Child1\n");
    }
};

class Child2 : public Parent {
public:
    virtual void doIt() {
        printf("Child2\n");
    }
};

class Child3 : public Parent, Parent2 {
public:
    virtual void doIt() {
        printf("Child3\n");
    }
    virtual void doIt2() {
        printf("Child3\n");
    }
};


int main() {

    Parent* p1 = new Child1();
    Parent* p2 = new Child2();
    Parent* p3 = new Child3();

    uint64_t one = rdtsc();
    Child1* c1 = dynamic_cast<Child1*>(p1);
    uint64_t two = rdtsc();
    printf("P1 -> C1: (%p)\t diff = %llu\n", c1, two - one);

    one = rdtsc();
    Child2* c2 = dynamic_cast<Child2*>(p2);
    two = rdtsc();
    printf("P2 -> C2: (%p)\t diff = %llu\n", c2, two - one);

    one = rdtsc();
    Child1* c1b = dynamic_cast<Child1*>(p2);
    two = rdtsc();
    printf("P2 -> C1: (%p)\t diff = %llu\n", c1b, two - one);

    one = rdtsc();
    Child3* c3 = dynamic_cast<Child3*>(p3);
    two = rdtsc();
    printf("P3 -> C3: (%p)\t diff = %llu\n", c3, two - one);

    one = rdtsc();
    Parent2* c3b = dynamic_cast<Parent2*>(p3);
    two = rdtsc();
    printf("P1 -> P2: (%p)\t diff = %llu\n", c3b, two - one);

    return 0;
}

