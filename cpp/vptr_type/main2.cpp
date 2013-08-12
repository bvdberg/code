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
    enum BaseKind {
        DERIVED1,
        DERIVED2,
        DERIVED2A,
    };
    Base (BaseKind k, int n_)
        : kind(k)
        , n(n_) {}
    virtual ~Base () {}
    BaseKind getKind() const { return kind; }

    virtual int get() { return n; }
private:
    BaseKind kind;
    int n;
};


class Derived1 : public Base {
public:
    Derived1(int n_) : Base(DERIVED1, n_) {}
    virtual int get() { return 1; }
    static bool classof(const Base *B) {
        return B->getKind() == DERIVED1;
    }
};


class Derived2 : public Base {
public:
    Derived2(BaseKind k, int n_) : Base(k, n_) {}
    Derived2(int n_) : Base(DERIVED2, n_) {}
    static bool classof(const Base *B) {
        switch (B->getKind()) {
        case DERIVED2:
        case DERIVED2A:
            return true;
        default:
            return false;
        }
    }
};


class Derived2A : public Derived2 {
public:
    Derived2A(int n_) : Derived2(DERIVED2A, n_) {}
    static bool classof(const Base *B) {
        return B->getKind() == DERIVED2A;
    }
};


template <class T> static inline bool isa(Base* b) {
    return T::classof(b);
}

// make different versions for different Base classes
template <class T> static inline T* cast(Base* b) {
    if (T::classof(b)) return static_cast<T*>(b);
    return 0;
}


int main(int argc, const char *argv[])
{
    uint64_t one = rdtsc();
    uint64_t two = rdtsc();
    printf("nothing: diff = %llu\n", two - one);

    Base* d1 = new Derived1(11);
    Base* d2 = new Derived2(21);
    Base* d2a = new Derived2A(11);

    one = rdtsc();
    Derived2A* dda1 = cast<Derived2A>(d2a);
    two = rdtsc();
    printf("cast1: diff = %llu\n", two - one);
    assert(dda1);

    one = rdtsc();
    Derived2A* dda2 = cast<Derived2A>(d2);
    two = rdtsc();
    printf("cast2: diff = %llu\n", two - one);
    assert(dda2 == 0);

    one = rdtsc();
    Derived2* dd2new = cast<Derived2>(d2a);
    two = rdtsc();
    printf("cast3: diff = %llu\n", two - one);
    // NOTE: does not work with multiple layers
    assert(dd2new);

    return 0;
}

