#include <stdio.h>

class Exception {};


class Base {
public:
    Base(int a) {
        printf("Base constructor\n");
        throw Exception();
    }
};


class Derived : public Base {
public:
    Derived()
    try
         : Base(10)
    {
        printf("Derived constructor\n");
    }
    catch (Exception& e) {
    //catch (...) {
        printf("Derived() : Caught Exception\n");
        throw;
    }
};


int main() {
    try {
        Derived d;
    } catch (Exception& e) {
        printf("main: Caught Exception\n");
    }
}

