#include <iostream>
#include <map>
#include <string>
using namespace std;

class Bar {
    int m;
public:
Bar(int i) : m(i){}
Bar& operator<(const Bar& rhs) const { return *this;}
//operator bool() const { return true; }
operator bool(const Bar& b) const { return true; }




};

int main() {

   map<Bar, int> m;

    m[Bar(3)] = 1;



   Bar a(3);
   Bar b(2);
bool c = a;
//    string s = a < b;
/*    
    if (a > b) {
        cout << "true" << endl;
    }*/
}


