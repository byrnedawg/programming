// Name: Gregory Byrne

#include <iostream>
#include <math.h>

using namespace std;


int trialDicvisionGCD(int a, int b)
{
    
    int gcd = 1;
    /*
    if(b > a)
    {
        swap(a,b);
    }
    */
    for(int i=2; i <= min(a,b); i++)
    {
        if(a % i == 0 && b % i == 0)
        {
            gcd = i;
        }
    }
    return gcd;
}

int recursiveGCD(int a, int b)
{
    if( b == 0)
    {
        return a;
    }
    
    return recursiveGCD(b, a % b);
    
}

int gcd(int a, int b)
{
    while(b != 0)
    {
        int temp = a;
        a = b;
        b = temp % b;
    }
    return a;
}

int lcm(int a, int b)
{
    return a * b / gcd(a, b);
}


int main()
{
    cout << "Trial by division GCD = " << trialDicvisionGCD(6,12) << "\n";
    cout << "Trial by division GCD = " << trialDicvisionGCD(12,18) << "\n";
    
    cout << "Recursive GCD = " << recursiveGCD(6,12) << "\n";
    cout << "Recursive GCD = " << recursiveGCD(12,18) << "\n";
    
    cout << "Final GCD = " << gcd(6,12) << "\n";
    cout << "Final GCD = " << gcd(12,18) << "\n";
    
    cout << "LCM = " << lcm(6,12) << "\n";
    cout << "LCM = " << lcm(12,18) << "\n";
    
    return 0;
}