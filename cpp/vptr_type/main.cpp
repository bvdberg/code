#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>

__inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
}


class Base {
public:
    Base (int n_)
        : n(n_) {}
    virtual ~Base () {}
    virtual int get() { return n; }
private:
    int n;
};

class Derived1 : public Base {
public:
    Derived1(int n_) : Base(n_) {}
    virtual int get() { return 1; }
};

class Derived2 : public Base {
public:
    Derived2(int n_) : Base(n_) {}
    //virtual int type() const { return DERIVED2; }
};

static Derived1 sd1(10);

static inline bool isDerived1(Base* b) {
    int* vptr = (int*)b;
    int* orig = (int*)&sd1;
    return (*orig == *vptr);
}


static void check(const char* name, Base* b) {
    printf("CHECKING %s\n", name);
    uint64_t one = rdtsc();
    bool same = isDerived1(b);
    uint64_t two = rdtsc();
    printf("mycast: diff = %llu\n", two - one);
    printf("%s is D1: %s\n", name, same ? "true" : "false");
}


int main(int argc, const char *argv[])
{
    uint64_t one = rdtsc();
    uint64_t two = rdtsc();
    printf("nothing: diff = %llu\n", two - one);

    Base* d1 = new Derived1(11);
    Base* d2 = new Derived2(21);
    Base* d2b = new Derived2(22);
    Base* b = new Base(3);
    check("base", b);
    check("d1", d1);
    check("d2", d2);
    check("d2b", d2b);

    return 0;
}

