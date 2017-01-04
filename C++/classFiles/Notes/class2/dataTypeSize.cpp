#include <iostream>
using namespace std;

int main()
{
   cout << "Size of char : " << sizeof(char) << endl;
   cout << "Size of int : " << sizeof(int) << endl;
   cout << "Size of short int : " << sizeof(short) << endl;
   cout << "Size of long : " << sizeof(long) << endl;
    cout << "Size of long long : " << sizeof(long long) << endl;
   cout << "Size of float : " << sizeof(float) << endl;
   cout << "Size of double : " << sizeof(double) << endl;
   cout << "Size of wchar_t : " << sizeof(wchar_t) << endl;
   cout << "lack of long long " << sizeof(unsigned long long) << endl;
   return 0;
}