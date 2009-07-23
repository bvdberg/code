#include <stdio.h>

class ClassA {
public:
    ClassA(int n) : num(n) {}
    ~ClassA() {}
    operator bool() {
        printf("bool operator called (num=%d)\n", num);
        return num==0;
    }

private:
    int num;
};

int main(int argc, char* argv[])
{
   ClassA a(0);
   if(a) printf("true\n");
   else printf("false\n");
    
   ClassA b(10);
   if(b) printf("true\n");
   else printf("false\n");

    (bool)a;
    if(a) printf("a=true\n");
    return 0;
}

