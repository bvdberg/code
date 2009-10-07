#ifndef CLASSA_H
#define CLASSA_H

#include <memory>

class ClassB;

class ClassA
{

public:
    ClassA();
    virtual ~ClassA();
    void init();

private:
   //const std::auto_ptr<ClassB> pb;
        //NOTE: use const if you initialize the var in the constructor
   std::auto_ptr<ClassB> pb;

    ClassA(const ClassA& toBeCopied);
    ClassA& operator=(const ClassA& toBeCopied);
};

#endif // CLASSA_H

