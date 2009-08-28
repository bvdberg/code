#ifndef CLASSA_H 
#define CLASSA_H

namespace Test {

class ClassB;

class ClassA {
public:
    ClassA();
    ~ClassA();
private:
    char array[100];
    ClassB* b;
    ClassA(const ClassA&);
    ClassA& operator= (const ClassA&);
};

}

#endif

