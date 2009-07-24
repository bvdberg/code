#include <deque>
#include <algorithm>

#include <stdio.h>

using namespace std;


class Player {};

class Error {};

class IntOperator {
public:
    virtual bool run(int) = 0;
};


class MyIntOperator : public IntOperator {
public:
    MyIntOperator(Player& player_) : player(player_) {}
    bool run(int num) {
        if (num % 4 == 0) return false;
        return true;
    }
private:
    Player& player;
};


class Deque {
public:
    typedef std::deque<int> Items;
    typedef Items::const_iterator ItemsConstIter;
    typedef Items::iterator ItemsIter;

    void addItem(int item) { items.push_back(item); }
    void removeItem(int item) {
        ItemsIter iter = find(items.begin(), items.end(), item);
        if (iter == items.end()) throw Error();
        items.erase(iter);
    }
    const Items getCopy() const { return items; }
    void forEachItem(IntOperator& function) {
        ItemsIter iter = items.begin();
        while (iter != items.end()) {
            if (!function.run(*iter)) iter = items.erase(iter);
            else ++iter;
        }
    }
private:
    Items items;
    Deque& operator=(const Deque&);
};


void print(char* str, Deque& container) {
    const Deque::Items items = container.getCopy();    
    Deque::ItemsConstIter iter = items.begin();
    printf("---- %s ----\n", str);
    while (iter != items.end()) {
        printf("%d\n", *iter);
        ++iter;
    }
}


int main() {
    int count = 20;
    Player player;
    Deque container;
    for (int i=0; i<count; ++i) { container.addItem(i); }

    // Old way
    const Deque::Items items = container.getCopy();    
    Deque::ItemsConstIter iter = items.begin();
    while (iter != items.end()) {
        if (*iter % 3 == 0)  container.removeItem(*iter);
        ++iter;
    }
    
    // New way
    print("before", container);
    MyIntOperator func(player);
    container.forEachItem(func);
    print("after", container);

    return 0;
}

