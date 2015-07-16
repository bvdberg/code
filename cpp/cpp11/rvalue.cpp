#include <iostream>

using namespace std;

class Foo {
public:
    Foo() : n(0) {
        cout << "Foo()\n";
    }
    ~Foo() {
        cout << "~Foo()\n";
    }
    Foo (const Foo& rhs) : n(rhs.n) {
        cout << "Foo(Foo&)\n";
    }
    Foo& operator= (const Foo& rhs) {
        cout << "operator=\n";
        n = rhs.n;
        return *this;
    }

    int n;
};

Foo createValue(int n) {
    Foo f;
    f.n = n;
    return f;
}

Foo& createRef(int n) {
    static Foo f;
    //Foo f;
    f.n = n;
    return f;   // returning stack variable by ref, but do anyway for example
}


void foo_examples() {
    {
        cout << "1\n";
        Foo f1;
    }

    {
        cout << "2\n";
        Foo f1;
        Foo f2 = f1;
    }

    {
        cout << "3\n";
        // NOTE: only single Foo is constructed and destroyed
        Foo f3 = createValue(2);
    }

    {
        cout << "4\n";
        // NOTE: only single Foo is constructed and destroyed
        // calls Foo(), ~Foo(), Foo(Foo&), ~Foo()
        Foo f3 = createRef(3);
    }

}


class Bar {
public:
    Bar() {
        cout << "Bar()\n";
        own = true;
    }
    ~Bar() {
        cout << "~Bar(own=" << own << "))\n";
    }
    Bar(const Bar& rhs) {   // normal copy constructor
        cout << "Bar(const Bar&)\n";
        own = false;
    }
    Bar(Bar&& rhs) {    // move constructor
        cout << "move Bar(const Bar&&)\n";
        rhs.own = false;
        own = true;

    }
    Bar& operator= (const Bar&) {   // normal assignment operator
        cout << "operator=\n";
        return *this;
    }
    Bar& operator= (Bar&& rhs) {    // move assignment operator
        cout << "move operator=\n";
        if (this != &rhs) {
            own = rhs.own;
            rhs.own = false;
        }
        return *this;
    }

    bool own;
};

Bar getBarValue() {
    Bar b;
    return b;
}

void bar_examples() {
    cout << "b1:\n";
    Bar b1;
    Bar b2(b1);

    Bar b3 = getBarValue();
}

int main(int argc, const char *argv[])
{
    foo_examples();
    bar_examples();
    return 0;
}

