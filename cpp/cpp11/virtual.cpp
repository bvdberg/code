#include <iostream>

class Base {
public:
    virtual void A(float = 0.0);
    virtual void B() const;
    virtual void C();

    virtual void D() final;
    virtual void E() final = 0; // HMM ok, but is weird!!
};

class Derived : public Base {
public:
    virtual void A(float) override; // OK
    virtual void B() const override;    // function profile must be exactly the same as base class!
    //virtual void D() override;    // error, no function to override

    void test() {
        int override = 10;      // OK since override is NOT a keyword, but an identifier
    }


    // virtual void D();    // error, since Base::D() is final!
};

// final class, error since there are abstract functions in Base
//class Final final : public Base { };

//class D2 : public Final {};     // error since Final is final


class Foo {
public:
    Foo(int n) {}       // custom constructor so no default constructor is created
    Foo() = default;    // re-install default constructor
    ~Foo() = default;   // ok
    Foo(const Foo&) = default;  // ok

    //Foo(const char* a) = default; // error, can only default functions that have a default

    // DELETE
    Foo(long long) = delete;    // dont allow (no implicit conversion to int version allowed)
    Foo& operator=(const Foo&) = delete;    // disallow assignment operator
};

int main(int argc, const char *argv[])
{
    return 0;
}

