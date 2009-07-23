
class Base {
protected:
int i;
};


class Derived : public Base {
public:
    void foo(Base& b) {
        i = 6;          //works
        int ii = b.i;   //<-- does not compile
    }
    void foo2(Derived& d) {
        int ii = d.i;   //works
    }
};

int main() {
    Derived d1;
    Derived d2;
    
    d1.foo(d2);
    d1.foo2(d2);

    return 0; 
}

