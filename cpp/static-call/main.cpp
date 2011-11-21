#include <stdio.h>
#include <string>

using namespace std;

class Test {
public:
    static void print() {
        printf("Test:print()\n");
    }
    void bar() {
        printf("Test:bar() value=%d\n", value);
    }
private:
    int value;
};


int main(int argc, const char *argv[])
{
    Test* t = NULL;
    Test::print();
    t->print();  // also allowed!
    t->bar();

    return 0;
}

