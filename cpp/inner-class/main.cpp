#include <stdio.h>

class Outer {
public:
    Outer(int i) : num(i) {}
    void run() {
        Inner in;
        in.run(this);
    }
private:
    int num;
    static const int snum = 123;

    class Inner {
    public: 
        void run(Outer* out) {
            printf("snum = %d\n", snum);        // doesn't even need Outer namespace
            printf("num = %d\n", out->num);
        }
    };
};

int main() {
    Outer out(10);
    out.run();

    return 0;
}

