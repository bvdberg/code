#include <iostream>
#include <string>
#include <vector>

struct Person {
    int age;
    std::string name;
};

static Person p1 = { 10, "bas" };   // ok in C++03

// ok in C++03
static Person ps[] = {
    { 20, "aa" },
    { 30, "bb" },
};

template <typename T> class MyArray {
private:
    std::vector<T> m_Array;

public:
    MyArray() { }

    MyArray(const std::initializer_list<T>& il)
    {
        // Manually populate the elements of the array from initializer_list x
        for (auto x: il) // use range-based for statement to iterate over the elements of the initializer list
            m_Array.push_back(x); // push them into the array manually
    }
};

// uniform initialization
void uniform_example() {
    int a[] = { 1, 3, 5, 7 };
    // note: missing '=', thus uniform initialization
    int b[] { 2, 4, 6, 8 };
}

int main(int argc, const char *argv[])
{
    Person p2 = { 20, "Iemand" }; // ok in C++03

    // only C++11:
    std::vector<int> myvector = { 2, 4, 6, 8 }; // error in C++03, ok in C++11

    MyArray<int> foo = { 3, 4, 6, 9 };
    MyArray<Person> pps = {
        { 40, "cc" },
        { 50, "dd" },
    };

    return 0;
}

