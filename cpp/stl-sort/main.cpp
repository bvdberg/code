#include <stdio.h>
#include <list>
#include <algorithm>
#include <string>

// PROBLEM: how to sort stl containers when using Pointer types

using namespace std;

class Elem {
public:
    Elem (int num_) : num(num_) {}
    void print() const {
        printf("  %d\n", num);
    }
    bool operator<(const Elem& rhs) const {
        return num < rhs.num;
    }
    static bool compare(const Elem* a, const Elem* b) {
        return (*a) < (*b);
    }
private:
    int num;
};


typedef std::list<Elem*> Elements;
typedef Elements::iterator ElementsIter;
static Elements elements;



static void print()
{
    printf("elements:\n");
    for (ElementsIter iter=elements.begin(); iter != elements.end(); ++iter) {
        (*iter)->print();
    }
}

int main() {
    elements.push_back(new Elem(10));
    elements.push_back(new Elem(12));
    elements.push_back(new Elem(3));
    elements.push_back(new Elem(8));
    elements.push_back(new Elem(14));
    elements.sort(Elem::compare);
    print();

    return 0;
}

