#include <stdio.h>
#include <string>

using namespace std;

class Interface {
public:
    Interface(const std::string& name_) : name(name_) {}
    virtual ~Interface () {}
    virtual void handle(Interface& i) = 0;
    const std::string& getName() const { return name; }
private:
    std::string name;
};


class ChildA : public Interface {
public:
    ChildA(const std::string& name_) : Interface(name_) {}
    virtual void handle(Interface& i) {
        printf("%s: %s(%s)\n", getName().c_str(), __func__, i.getName().c_str());
    }
};


class ChildB : public Interface {
public:
    ChildB(const std::string& name_) : Interface(name_) {}
    virtual void handle(Interface& i) {
        printf("%s: %s(%s)\n", getName().c_str(), __func__, i.getName().c_str());
    }
};


int main(int argc, const char *argv[])
{
    Interface* a1 = new ChildA("A1");
    Interface* a2 = new ChildA("A2");
    Interface* b1 = new ChildB("B1");
    Interface* b2 = new ChildB("B2");

    a1->handle(*a2);
    a2->handle(*b1);
    b1->handle(*a1);
    b2->handle(*b1);

    delete a1;
    delete a2;
    delete b1;
    delete b2;
    return 0;
}

