#include <stdio.h>

template <typename T> void function(T a, T b) {
    if (a == b) printf("equal\n");
    else printf("not equal\n");
}


class IPoint {
public:
    IPoint() : x(0), y(0) {}
    IPoint(int x_, int y_) : x(x_), y(y_) {}
    //bool operator< (const IPoint& rhs) const;
    bool operator== (const IPoint& rhs) const {
        return (x == rhs.x && y == rhs.y);
    }
    int x;
    int y;
};


int main() {
    int a = 10;
    int b = 20;
    function(a, b);
    function(a, a);

    IPoint aa(0, 0);
    IPoint bb(1, 1);
    function(aa, bb);
    return 0;
}

