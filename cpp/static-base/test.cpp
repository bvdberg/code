#include <stdio.h>
// Both derived classes will have the same static variable in the base

class Base {
public:
    static int constant;
};

int Base::constant;

class Derived1 : public Base {
public:
    int getNum() { return constant; }
    void setNum(int n) { constant = n; }
};

class Derived2 : public Base {
public:
    int getNum() { return constant; }
    void setNum(int n) { constant = n; }
};

int main() {
    Derived1 d1;
    d1.setNum(10);

    Derived2 d2;
    d1.setNum(20);
    printf("d1->%d, d2->%d\n", d1.getNum(), d2.getNum());
    return 0;
}
