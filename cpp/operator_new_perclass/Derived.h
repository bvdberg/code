#ifndef DERIVED_H
#define DERIVED_H

#include "Base.h"
#include <stdint.h>

class Derived : public Base {
public:
    Derived(int id_);
    ~Derived();
private:
    uint32_t mydata[40];
};

#endif

