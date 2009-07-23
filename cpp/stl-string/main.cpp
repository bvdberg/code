#include <string>

using namespace std;

int main(int argc, char* argv[])
{

    const char* cp = 0;
//    string a = cp;  //throws exception
    string a = cp ? cp : "";
    printf("size = %d\n", a.size());



    return 0;
}

