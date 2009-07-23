#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ofstream myfile;
    myfile.open ("example.txt");
    myfile << "Writing this to a file.\n";
    myfile.close();

    string line;
    ifstream myfile2 ("example.txt");
    if (myfile2.is_open()) {
        while (! myfile2.eof() ) {
        getline (myfile2, line);
            cout << line << endl;
        }
        myfile2.close();
    } else cout << "Unable to open file"; 

    return 0;
}

