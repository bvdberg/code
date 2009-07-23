#ifndef LIB1_H 
#define LIB1_H

class Base {
public:
    virtual void myprint() = 0;
    virtual void func2() = 0;
};

class Derived : public Base {
public:
    virtual void myprint();
    virtual void func2();
};


#endif

