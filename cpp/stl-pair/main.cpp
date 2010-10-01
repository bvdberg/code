#include <stdio.h>
#include <map>
#include <string>
#include <utility>

using namespace std;

class MyClass {
public:
    void add(const std::string& s1, const std::string& s2, int num) {
        if (find(s1, s2)) {
            printf("<%s,%s> already known\n", s1.c_str(), s2.c_str());
            return;
        }
        db[MyPair(s1, s2)] = num;
    }
    bool find(const std::string& s1, const std::string& s2) const {
        DBConstIter iter = db.find(MyPair(s1, s2));
        return (iter != db.end());
    }
    void print() const {
        for (DBConstIter iter = db.begin(); iter != db.end(); ++iter) {
            const MyPair* mp = &(iter->first);
            printf("<%s,%s> = %d\n", mp->first.c_str(), mp->second.c_str(), iter->second);
        }
    }

    typedef std::pair<std::string, std::string> MyPair;
    typedef std::map<MyPair, int> DB;
    typedef DB::const_iterator DBConstIter;
    typedef DB::iterator DBIter;
    DB db;
};


int main() {
    MyClass mc;
    mc.add("goblin", "area1", 10);
    mc.add("orc", "area2", 11);
    mc.add("giant", "area2", 12);
    mc.add("bear", "area1", 13);
    mc.print();
    mc.add("orc", "area2", 11);

    return 0;
}

