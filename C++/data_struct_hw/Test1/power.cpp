// Name: Gregory Byrne
// Assignment: HW2 - Miller Rabin
#include <iostream>

using namespace std;




int power(int x, int n)
{
    int prod = 1;
    while(n > 0)
    {
        if(n % 2 != 0)
        {
            prod = prod * x;
        }
        n = n / 2;
        x = x * x;
    }
    return prod;
}

int powerMod(unsigned long long x, int n, int m)  // O(log n)
{ 
    int prod = 1;
    while(n > 0)
    {
        if(n % 2 != 0)
        {
            prod = (prod * x) % m;
        }
        n = n / 2;
        x = (x * x) % m;
    }
    return prod;
}

int main()
{
    cout << "Power = " << power(2,8) << "\n";
    cout << "Power Mod = " << powerMod(2,8,4) << "\n";
    return 0;
}