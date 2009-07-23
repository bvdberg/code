#include <stdio.h>

/*
    compile with:
    g++ main.cpp --dump-class-hierarchy
    see main.class to see vtables

*/

class Base1 {
public:
    virtual void foo() {}
};

class Base2 {
public:
    virtual void bar() {
        printf("Base2 this = %p\n", this);
    }
    Base2* getThis() {
        return this;
    }
};

class Derived : public Base1, public Base2 {
public:
    virtual void foo() {
        printf("this=%p,  (B1*)this=%p,  (B2*)this=%p\n", this, (Base1*)this, (Base2*)this);
    }
};

int main() {

    Base1* d1 = new Derived();
    printf("d1 = %p\n", d1);

    Base2* b1 = (Base2*)d1;
    printf("normal cast: %p -> %p\n", d1, b1);

    Base2* b2 = dynamic_cast<Base2*>(d1);
    printf("dynamic cast: %p -> %p\n", d1, b2);

    d1->foo();
    b2->bar();

    return 0;
}

