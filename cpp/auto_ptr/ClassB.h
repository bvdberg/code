#ifndef CLASSB_H
#define CLASSB_H

class ClassB
{

public:
    ClassB();
    virtual ~ClassB();

private:
    ClassB(const ClassB& toBeCopied);
    ClassB& operator=(const ClassB& toBeCopied);
};

#endif // CLASSB_H

