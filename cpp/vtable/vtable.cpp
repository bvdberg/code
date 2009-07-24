// compile with g++ vtable.cpp -fdump-class-hierarchy

class Base {
public:
    virtual void foo() {}
    virtual void bar() {}
};


class Child1 : public Base {
public:
    virtual void foo() {}
    virtual void bar() {}
    virtual void son() {}
};


class Child2 : public Child1 {
public:
    virtual void foo() {}
    virtual void bar() {}
    virtual void son() {}
    virtual void doe() {}
};

int main() {

    return 0;
}

