#ifndef FOO_H
#define FOO_H

class Foo {
public:
    Foo (int x_) : x(x_), y(2*x) {}
    void print() const;
private:
    int x;
    int y;
};

#endif

