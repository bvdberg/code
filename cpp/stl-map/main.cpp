#include <stdio.h>
#include <map>

using namespace std;

class Container {
public:
    void add(int key, int value) {
        numbers[key] = value;
        printf("adding %2d -> %2d  size=%d\n", key, value, numbers.size());
    }
    void remove(int value) {
        NumbersIter iter = numbers.find(value);
        if (iter == numbers.end()) {
            printf("%s() NO SUCH ELEMENT (%d)\n", __func__, value);
        } else {
            numbers.erase(iter);
        }
    }
    void print() {
        printf("--map--\n");
        for (NumbersIter iter = numbers.begin(); iter != numbers.end(); ++iter) {
            printf("  %2d -> %2d\n", iter->first, iter->second);
        }
    }
private:
    typedef std::map<int, int> Numbers;
    typedef Numbers::iterator NumbersIter;
    Numbers numbers;
};

int main() {
    Container con;
    con.add(5, 10);
    con.add(7, 14);
    con.add(10, 20);

    con.print();

    return 0;
}

