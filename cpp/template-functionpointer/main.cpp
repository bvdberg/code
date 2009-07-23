#include <stdio.h>

class ClassA {
public:
    virtual void print() = 0;
};


class ClassB : public ClassA {
public:
    ClassB() {
        printf("%s\n", __PRETTY_FUNCTION__);
        if (function) function(this);
        else printf("  no function\n");
    }
    virtual void print() { printf("%s\n", __PRETTY_FUNCTION__); }
    void onlyB() { printf("%s\n", __PRETTY_FUNCTION__); }

    typedef void(*Function)(ClassB*);
    static Function function; 
};

ClassB::Function ClassB::function = 0;


class ClassC : public ClassA {
public:
    typedef void(*Function)(ClassC*);
    ClassC(Function f) {
        f(this);
    }
    ClassC() {
        printf("%s\n", __PRETTY_FUNCTION__);
        if (function) function(this);
    }
    virtual void print() { printf("%s\n", __PRETTY_FUNCTION__); }
    void c() { printf("%s\n", __PRETTY_FUNCTION__); }
    static Function function; 
};

ClassC::Function ClassC::function = 0;


// special function for ClassB
void funcB(ClassB* b) {
    printf("  SPECIAL(B) %s -> ", __PRETTY_FUNCTION__);
    b->onlyB();
}

// generic template function
template <typename T>
void function1(T* t) { printf("  TEMPLATE(T) %s -> ", __PRETTY_FUNCTION__); t->print(); }


int main() {
    // 3 ClassB instantiations: none, custom and generic function
    ClassB::function = 0;
    ClassB b1;
    
    ClassB::function = funcB; 
    ClassB b2;

    ClassB::function = function1<ClassB>;   //<- template function as custom function
    ClassB b3;

    printf("\n");

    ClassC::function = function1<ClassC>;   //<- same template function as different custom function
    ClassC c1;

    return 0;
}

