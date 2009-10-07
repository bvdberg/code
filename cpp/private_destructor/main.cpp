#include <stdio.h>
class Test {
public:
    Test() : refcount(1) {
        printf("%s()\n", __func__);
    }
private:
    virtual ~Test () {
        printf("%s()\n", __func__);
    }

public:
    void get() { refcount++; }
    void release() {
        refcount--;
        if (refcount == 0) delete this;
    }
private:
    int refcount;
};


int main(int argc, const char *argv[])
{
    // NOTE: not allowed to put object on stack if destr is private
//    Test test;    // <- not allowed

    Test* t2 = new Test();
    t2->release();
    return 0;
}

