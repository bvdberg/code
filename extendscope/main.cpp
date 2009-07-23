#include <stdio.h>

class Base {
protected:
    void print() {
        printf("Base::print()\n");
    }
};


class Derived : public Base {
public:
    using Base::print;
};


int main() {
    Derived d;
    d.print();

    Base* bp = new Derived();
    //bp->print();      //error
    return 0;
}

