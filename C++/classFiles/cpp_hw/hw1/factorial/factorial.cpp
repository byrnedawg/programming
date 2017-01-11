// Name: Gregory Byrne
// Assignment: HW1e n factorial

#include <iostream>

using namespace std;

unsigned int x;

unsigned long long factorial(unsigned int n)
{
   unsigned long long fact_n = n; //long long the biggest factorial calculated is 20

    while(n > 1)
    {
        fact_n *=  n-1;
        n--;
        //cout << fact_n << " "<<endl;
    }

    return fact_n;
}

int main()
{
    cout << "Enter in number for n " << endl;
    cin >> x;
    cout << x << " Function is = " << factorial(x) << endl;

    return 0;
}
