#include <stdio.h>
#include <iostream>

using namespace std;

void old_enums() {
   enum Color {
        RED,
        BLUE
    };

    enum Fruit {
        BANANA,
        APPLE
    };

    Color a = RED;
    Fruit b = BANANA;

    //Fruit c = 0;  // error. In C, OK
    //Fruit d = RED;  // error. In C, warning (Wconversion)
    //Fruit e = (int)RED;  // error, in C, OK

#if 0
    if (a == b) { // The compiler will compare a and b as integers
        //  NOTE: clang gives waring (-Wenum-compare)
        cout << "a and b are equal" << endl; // and find they are equal!
    } else {
        cout << "a and b are not equal" << endl;
    }
#endif
}

void new_enums() {
    enum class Color {
        RED,
        BLUE
    };

    enum class Fruit {
        BANANA,
        APPLE
    };

    Color a = Color::RED; // note: RED is not accessible any more, we have to use Color::RED
    Fruit b = Fruit::BANANA; // note: BANANA is not accessible any more, we have to use Fruit::BANANA

    //Color c = 10; // error

    // NOTE: since Color and Fruit are types, you can define your ower comparison operator
#if 0
    // clang 3.6.2, warning + error
    if (a == b) // compile error here, as the compiler doesn't know how to compare different types Color and Fruit
        cout << "a and b are equal" << endl;
    else
        cout << "a and b are not equal" << endl;
#endif
}


int main(int argc, const char *argv[])
{
    old_enums();
    return 0;
}

