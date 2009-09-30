#ifndef CLASSB_H 
#define CLASSB_H

namespace Test {

class ClassB {
public:
    ClassB();
    ~ClassB();
private:
    char array[30];
    ClassB(const ClassB&);
    ClassB& operator= (const ClassB&);
};

}

#endif

