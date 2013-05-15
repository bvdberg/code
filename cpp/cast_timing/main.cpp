#include <stdio.h>
#include <string>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>

__inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
}

class Visitor;


class Base {
public:
    virtual const char* getName() = 0;
    virtual void accept(Visitor& v) = 0;
};


class Derived1 : public Base {
public:
    Derived1(const char* n) : name(n) {}
    virtual const char* getName() { return name.c_str(); }
    virtual void accept(Visitor& v);
private:
    std::string name;
};


class Derived2 : public Base {
public:
    Derived2(const char* n) : name(n) {}
    virtual const char* getName() { return name.c_str(); }
    virtual void accept(Visitor& v);
private:
    std::string name;
};


class Visitor {
public:
    virtual ~Visitor() {}
    virtual void visit(Base&) { assert(0); }
    virtual void visit(Derived1&) {}
    virtual void visit(Derived2&) {}
};

#define VISITOR_ACCEPT(a) void a::accept(Visitor& v) { v.visit(*this); }

VISITOR_ACCEPT(Derived1);
VISITOR_ACCEPT(Derived2);

template <class T> class Caster : public Visitor {
public:
    virtual void visit(T& node_) {
        node = &node_;
    }
    static T* get(Base* node_) {
        Caster<T> visitor(*node_);
        return visitor.node;
    }
private:
    Caster(Base& n) : node(0) {
        n.accept(*this);
    }
    T* node;
};


int main(int argc, const char *argv[])
{
    uint64_t one = rdtsc();
    uint64_t two = rdtsc();
    printf("nothing: diff = %llu\n", two - one);
    
    Base* b1 = new Derived1("one");
    Base* b2 = new Derived2("two");

    one = rdtsc();
    Derived1* d1 = dynamic_cast<Derived1*>(b1);
    two = rdtsc();
    printf("dynamic cast diff = %llu\n", two - one);

    one = rdtsc();
    Derived1* d1b = reinterpret_cast<Derived1*>(b1);
    two = rdtsc();
    printf("reinterpret cast diff = %llu\n", two - one);

    one = rdtsc();
    Derived1* d1c = Caster<Derived1>::get(b1);
    two = rdtsc();
    printf("cast visitor diff = %llu\n", two - one);

    return 0;
}

