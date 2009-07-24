#ifndef CLASSA_H
#define CLASSA_H

class ClassA
{
    // Allows the test class to access the private members.
    friend class TestClassA;

public:
    ClassA(int a_);
    virtual ~ClassA();
    operator int() const;
    ClassA& operator= (char* s);

private:
    ClassA(const ClassA& toBeCopied);
//    ClassA& operator=(const ClassA& toBeCopied);

    int a;
};


#endif // CLASSA_H

