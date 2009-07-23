#ifndef CLASSA_H
#define CLASSA_H

class ClassA
{
public:
    ClassA(int a = 0, int b = 1);
    ClassA& operator=(int a);
private:
    //ClassA(const ClassA& toBeCopied);
    //ClassA& operator=(const ClassA& toBeCopied);

    int m;
    int n;
};

#endif // CLASSA_H

