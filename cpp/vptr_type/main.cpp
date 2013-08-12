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

#define ISA(Base) \
static inline bool classof(Base* b) { \
    int* vptr = (int*)b; \
    int* orig = (int*)&_type; \
    return (*orig == *vptr); \
}\


class Derived1 : public Base {
public:
    Derived1(int n_) : Base(n_) {}
    virtual int get() { return 1; }

    ISA(Base);
private:
    static Derived1 _type;
    Derived1() : Base(0) {}
};
Derived1 Derived1::_type;

class Derived2 : public Base {
public:
    Derived2(int n_) : Base(n_) {}
    ISA(Base);
private:
    static Derived2 _type;
    Derived2() : Base(0) {}
};
Derived2 Derived2::_type;


class Derived2A : public Derived2 {
public:
    Derived2A(int n_) : Derived2(n_) {}
    ISA(Base);
    ISA(Derived2);
private:
    static Derived2A _type;
    Derived2A() : Derived2(0) {}
};
Derived2A Derived2A::_type;




template <class T> static inline bool isa(Base* b) {
    return T::classof(b);
}

// make different versions for different Base classes
template <class T> static inline T* cast(Base* b) {
    if (T::classof(b)) return static_cast<T*>(b);
    return 0;
}


static void check(const char* name, Base* b) {
    uint64_t one = rdtsc();
    Derived1* dd1 = cast<Derived1>(b);
    uint64_t two = rdtsc();
    printf("diff = %4llu -> ", two - one);
    printf("%s %s\n", name, dd1 ? "true" : "false");
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
    printf("\n");
    check("base", b);
    check("d1", d1);
    check("d2", d2);
    check("d2b", d2b);

    Base* dd2 = new Derived2A(11);
    one = rdtsc();
    Derived2A* dda1 = cast<Derived2A>(dd2);
    two = rdtsc();
    printf("cast1: diff = %llu\n", two - one);
    assert(dda1);

    one = rdtsc();
    Derived2A* dda2 = cast<Derived2A>(d2);
    two = rdtsc();
    printf("cast1: diff = %llu\n", two - one);
    assert(dda2 == 0);

    return 0;
}

