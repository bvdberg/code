#include <stdio.h>


class BaseA {
public:
    void doFoo() { printf("BaseA::doFoo()\n"); }
    void doBar() { printf("BaseA::doBar()\n"); }
};


class BaseB {
public:
    void doFoo() { printf("BaseB::doFoo()\n"); }
};


template <typename T> class TempA : public T {
public:
    void doSomething() {
        T::doFoo();   
    }
    void doExtended() {
        T::doFoo();
        T::doBar();
    }
};


int main() {
    TempA<BaseA> a;
    a.doSomething();
    a.doExtended();
   
    TempA<BaseB> b;
    b.doSomething();
    //b.doExtended();
    
    return 0;
}

