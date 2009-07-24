#include <string>
#include <stdio.h>

using namespace std;

class Outer {
public:
    Outer(string n) : name(n) {}
    class Inner {
    public:
        void method(const Outer& out) {
            //NOTE: Inner can access Outer's privates.
            printf("outer name = %s\n", out.name.c_str());
        }
    };
    Inner inner;
private:
    string name;
};


int main() {
    Outer out("foo");
    Outer::Inner in;
    in.method(out);

    return 0;
}

