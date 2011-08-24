#include <stdio.h>
// NOTE: NEEDED FOR PLACEMENT OPERATOR
#include <new>

class MyClass {
public:
    MyClass(int num_) : num(num_) {}
    void print() const {
        printf("%p   num=%d\n", this, num);
    }
private:
    int num;
};


int main(int argc, const char *argv[])
{
    char* buf = new char[sizeof(MyClass)];
    printf("buf = %p\n", buf);
    // un-initialized
    MyClass* myclass = (MyClass*)buf;
    myclass->print();

    // initialized
    MyClass* mc = new(buf) MyClass(10);
    mc->print();

    delete[] buf;

    // array
    int num = 10;
    char* buf2 = new char[num  * sizeof(MyClass)];
    MyClass* mcArray = (MyClass*)buf2;
    // un-initialized
    for (int i=0; i<num; i++) {
        printf("i=%2d: ", i);
        mcArray[i].print();
    }
    printf("\n");
    for (int i=0; i<num; i++) {
        new(&mcArray[i]) MyClass(i);
    }
    for (int i=0; i<num; i++) {
        printf("i=%2d: ", i);
        mcArray[i].print();
    }

    delete[] buf2;

    return 0;
};
