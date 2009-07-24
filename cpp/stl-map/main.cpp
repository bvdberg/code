#include <stdio.h>
#include <map>

using namespace std;

class Container {
public:
    void add(int key, int value) {
        numbers[key] = value;
        printf("adding %d->%d  size=%d\n", key, value, numbers.size());
    }
    void print() {
        for (NumbersIter iter = numbers.begin(); iter != numbers.end(); ++iter) {
            printf("Contains: %d -> %d\n", iter->first, iter->second);
        }

/*
        for (NumbersIter iter = numbers.begin(); iter != numbers.end(); ++iter) {
            printf("at element: %d -> %d\n", iter->first, iter->second);
            if (iter->first == 5) {
                printf("deleting %d\n", iter->first);
                numbers.erase(iter);
            }
            printf(" contains: %d -> %d\n", iter->first, iter->second);
        }
*/
        NumbersIter iter = numbers.begin();
        while (iter != numbers.end()) {
            printf("at element: %d\n", iter->first);
            if (iter->first == 5) {
                printf("deleting %d\n", iter->first);
                numbers.erase(iter);
            }
            iter++;
        }

        for (NumbersIter iter = numbers.begin(); iter != numbers.end(); ++iter) {
            printf("Contains: %d -> %d\n", iter->first, iter->second);
        }
    }
private:
    typedef std::map<int, int> Numbers;
    typedef Numbers::iterator NumbersIter;
    Numbers numbers;
};

int main() {
    Container con;
    con.add(5,10);
    con.add(7,14);
    con.add(10,20);

    con.print();

    return 0;
}

