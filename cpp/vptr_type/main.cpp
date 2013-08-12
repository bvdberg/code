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
    // NOTE: only works with 1 layer of derived classes!
    // for more layers, more checks need to be done (if orig == a || orig == b etc)
    static inline bool isa(Base* b) {
        int* vptr = (int*)b;
        int* orig = (int*)&_type;
        return (*orig == *vptr);
    }
private:
    static Derived1 _type;
    Derived1() : Base(0) {}
};

Derived1 Derived1::_type;

class Derived2 : public Base {
public:
    Derived2(int n_) : Base(n_) {}
    static inline bool isa(Base* b) {
        int* vptr = (int*)b;
        int* orig = (int*)&_type;
        return (*orig == *vptr);
    }
private:
    static Derived2 _type;
    Derived2() : Base(0) {}
};

Derived2 Derived2::_type;





template <class T> static inline bool isa(Base* b) {
    return T::isa(b);
}

// make different versions for different Base classes
template <class T> static inline T* cast(Base* b) {
    if (T::isa(b)) return static_cast<T*>(b);
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

    Derived1* dd1 = cast<Derived1>(d2);
    assert(dd1==0);
    Derived2* dd2 = cast<Derived2>(d2);
    assert(dd2);

    return 0;
}

