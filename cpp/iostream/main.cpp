#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, const char *argv[])
{
    std::cout << "Hallo: " << argc << std::endl;
    int a;
    cin >> a;
    cout << "a=" << a << endl;
    cout << "name: ";
    string name;
    cin >> name;
    cout << "your name is '" << name << "'\n";

    string mystr("1234");
    int myint;
    stringstream(mystr) >> myint;
    cout << "myint = " << myint << '\n';

    return 0;
}

