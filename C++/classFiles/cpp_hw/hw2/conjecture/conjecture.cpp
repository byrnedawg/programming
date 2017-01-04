// Name: Gregory Byrne
// Assignment: HW1a 3n+1 conjecture
#include <iostream>
using namespace std;

int main() 
{
    unsigned int n;
    cout << "Enter in number for n " << endl;
    cin >> n;
   while(n != 1)
   {
       if(n % 2 == 0)
       {
           n = n / 2;
           cout << "Even divide n by 2 n = " << n << endl;
       }
       else
       {
           n = n*3 + 1;
           cout << "Odd multiply by 3 than add 1 n = " << n << endl;
       }
   }
   
   cout << "n has finally converged to 1 so we are done" << endl;
   return 0;
}
