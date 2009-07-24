#include <stdio.h>

class Base {
public:
    Base(int n) : number(n) {}
    Base(const Base& rhs) : number(rhs.number) {}
    virtual void print() {
        printf("Base: number = %d\n", number);
    }
protected:
    int number;
};

class Derived : public Base {
public:
    Derived(int n) : Base(n) {}
    virtual void print() {
        printf("Derived: number = %d\n", number);
    }
};


int main() {
    Base b(10);
    b.print();

    Derived d(20);
    d.print();

    Derived d2(d);  // works, generated copy constructor works
    d2.print();

    return 0;
}

