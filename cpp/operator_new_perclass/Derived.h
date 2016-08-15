#ifndef DERIVED_H
#define DERIVED_H

#include "Base.h"
#include <stdint.h>

class Derived : public Base {
public:
    Derived(int id_);
    virtual ~Derived();
private:
    uint8_t mydata[20];
};

#endif

