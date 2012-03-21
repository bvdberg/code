#include <stdio.h>
#include <set>

typedef std::set<int> Numbers;
typedef Numbers::iterator NumbersIter;
Numbers numbers;

void find(int num) {
    NumbersIter iter = numbers.find(num);
    if (iter == numbers.end()) {
        printf("%d not found\n", num);
    } else {
        printf("%d found\n", num);
    }
}

void add(int num) {
    numbers.insert(num);        
}

void remove(int num) {
    NumbersIter iter = numbers.find(num);
    if (iter != numbers.end()) numbers.erase(iter);
}

void iterate() {
    printf("size = %d\n", numbers.size());
    NumbersIter iter;
    for (iter = numbers.begin(); iter != numbers.end(); ++iter) {
        printf("  %d\n", *iter);
    }
}

int main(int argc, const char *argv[])
{
    find(10);
    add(10);
    add(1);
    add(6);
    add(15);
    find(10);
    iterate();
    remove(10);
    find(10);
    
    return 0;
}

