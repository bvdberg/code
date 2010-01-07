#include <stdio.h>

class Interface {
public:
    virtual ~Interface () {}
protected:
    void handle() {     // <-- scope protected
        printf("Interface::%s()\n", __func__);
    }
};

class Child : public Interface {
public:
    void handle() {     // <-- scope now public
        printf("Child::%s()\n", __func__);
    }
};

int main(int argc, const char *argv[])
{
    Child* c = new Child();
    c->handle();    

    Interface* i = c;
//    i->handle();   <-- does not compile
    return 0;
}

