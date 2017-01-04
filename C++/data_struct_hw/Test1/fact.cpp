// Name: Gregory Byrne

#include <iostream>

using namespace std;


int fact(int n)
{
    int f = 1; 
    for(int i = 1; i <= n; i++)
    {
        f *= i;
    }
    return f;
    
}

int recursiveFact(int n)
{
    if( n <= 1)
    {
        return 1;
    }
    
    return n* recursiveFact(n-1);
    
}



int main()
{
    cout << "Brute Fibo = " << fact(6) << "\n";
    cout << "Brute Fibo = " << fact(5) << "\n";
    
    cout << "Recursive Fibo = " << recursiveFact(6) << "\n";
    cout << "Recursive Fibo = " << recursiveFact(5) << "\n";
    
    return 0;
}