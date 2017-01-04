#include <iostream>
#include <fstream>

using namespace std;

int main(){
    ofstream f("myfile.txt");
    f << "test" << 5 << " " << 2.5*3;
    f << flush;
    f.close();
    ifstream g("myfile.txt");
    string s1, s2;
    g >> s1;
    g >> s2;
    cout << s1 << endl;
    cout << s2 << endl;
    return 0;
}