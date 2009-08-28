#include "ClassA.h"
#include "ClassB.h"

using namespace Test;

ClassA::ClassA() {
    b = new ClassB();
}

ClassA::~ClassA() {
    delete b;
}

