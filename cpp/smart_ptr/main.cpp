#include <stdio.h>

class Data {
public:
    Data(int i) : number(i) {
        printf("created %d\n", number);
    }
    ~Data() {
        printf("destroyed %d\n", number);
    }
private:
    int number;
};

class SmartPtr {
public:
    SmartPtr(Data* d) : ptr(d) {}
    ~SmartPtr() {
        delete ptr;
    }
private:
    Data* ptr;
};


SmartPtr func1(int i) {
    Data* d = new Data(i);
    SmartPtr smart(d);
    return smart;
}

int main(int argc, const char *argv[])
{
    printf("one\n");
    {
        SmartPtr ptr = func1(1);
        printf("two\n");
    }
    printf("three\n");
    return 0;
}

