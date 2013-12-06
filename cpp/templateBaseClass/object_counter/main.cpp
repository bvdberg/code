#include <iostream>

// By using the CRTP (Curiously Recurring Template Pattern) we can add counter features
// to any class

using namespace std;

template <typename T>
struct counter
{
    static int created;
    static int alive;

    counter()
    {
        ++created;
        ++alive;
    }

    counter(const counter&)
    {
        ++created;
        ++alive;
    }
protected:
    ~counter() // objects should never be removed through pointers of this type
    {
        --alive;
    }
};
template <typename T> int counter<T>::created( 0 );
template <typename T> int counter<T>::alive( 0 );

class X : counter<X>
{
    // ...
};


class Y : counter<Y>
{
    // ...
};

int main(int argc, const char *argv[])
{
    X x1;
    X x2;
    {
        X x3;
    }
    cout << "X: created: " << counter<X>::created << " alive: " << counter<X>::alive << '\n';
    return 0;
}

