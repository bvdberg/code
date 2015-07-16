#include <stdio.h>
#include <list>
#include <iostream>

static char* func() {
    return 0;
}

void auto_example() {
    auto a = 10;        // ok!?
    auto b = -20;           // ok
    //auto int c = 30;    // warning (auto ..)
    auto c = "hello";

    auto d = func();

    // iterators
    typedef std::list<int> MyList;
    MyList l;
    auto iter = l.begin();
    const auto iter2 = l.end();
}

void decltype_example() {
    // decltype
    decltype("hallo") e = "hello"; // type of e is 'const char[6]'
    const char* f = e;      // ok, converts char[6] into char*
}

void nullptr_example() {
    const char* a = 0;      // ok
    const char* b = NULL;   // ok
    const char* c = nullptr;    // C++11 way

    if (c == nullptr) {

    }
    std::nullptr_t d = nullptr; // nullptr_t is type of nullptr
    //if (d) { }      // get -Wconversion
}

void range_based_for() {
    typedef std::list<int> MyList;
    MyList l;

    // traditional
    for (MyList::iterator it = l.begin(); it != l.end(); ++it) { }
    // with auto
    for (auto it = l.begin(); it != l.end(); ++it) { }
    // range based
    for (auto x: l) { }    // x is read-only
    // if you want to modify x
    for (auto& x: l) {
        std::cout << x;
    }
    // in between, hmm doesn't work
    //for (MyList::const_iterator it: l) {}
}

void static_assert_example() {
    static_assert(sizeof(int) == 4, "not 64-bit");
    static_assert(__cplusplus > 199711L, "Program requires C++11 capable compiler");
}

// constexpr examples
constexpr int getval() { return 5; }
const int a = 10;
const int b = a + 20;
//const int c = getval;

int array[getval() + 3];    // only allowed if getval is constexpr

int main(int argc, const char *argv[])
{
    return 0;
}

