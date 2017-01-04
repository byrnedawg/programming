#include <iostream>
using namespace std;

unsigned long long x;

bool isPrime(unsigned long long p) 
{
    unsigned long long divcount = 0;
    for(int i = 1; i <= p; i++)
    {
        if(p % i == 0)
        {
            divcount++;
            
            if(divcount > 2)
            {
                return  false;
            }
        }
    }
    if(divcount < 2)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int main() 
{
    cout << "Enter in number for prime # check " << endl;
    cin >> x;
    
    if(isPrime(x))
    {
        cout << x << " is a prime number";
    }
    else
    {
        cout << x << " is not a prime number";
    }
    
    return 0;
}
