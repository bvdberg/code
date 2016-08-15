#ifndef FOO_H
#define FOO_H

#include "Base.h"
#include <stdint.h>

class Foo : public Base {
public:
    Foo(int id_);
    ~Foo();

    int getID() const { return id; }
private:
    uint8_t mydata[10];
};

#endif

