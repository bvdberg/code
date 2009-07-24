#include <stdio.h>

class Base {
public:
    virtual void function() { printf("Base\n"); }
};

class Derived1 : public Base {
public:
    virtual void function() { printf("D1\n"); }
};

class Derived2 : public Derived1 {
public:
    virtual void function() { printf("D1\n"); }
};


int main() {
    printf("Base = %u\n", sizeof(Base));
    printf("Derived1 = %u\n", sizeof(Derived1));
    printf("Derived2 = %u\n", sizeof(Derived2));
    return 0;
}

