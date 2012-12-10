#include <stdio.h>
#include <memory>


class MyClass {
public:
    MyClass(int i) : num(i) {
        printf("created %d\n", num);
    }
    ~MyClass() {
        printf("deleted %d\n", num);
    }
    int num;
};

typedef std::auto_ptr<MyClass> Result;

Result func1(int i) {
    Result ptr(new MyClass(i));
    return ptr;
}

Result func2(Result r) {
    return r; 
}

int main(int argc, const char *argv[])
{
    {
        printf("=== normal scope ===\n");
        Result ptr(new MyClass(1));
    }
    
    {
        printf("=== return type ===\n");
        Result res = func1(2);
    }

    {
        printf("=== pass through function ===\n");
        Result res = func1(3);
        Result res2 = func2(res);
    }
    return 0;
}

