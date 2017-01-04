// Name: Gregory Byrne

#include <iostream>

using namespace std;


int fibo(int n)
{
    int a = 0; 
    int b = 1; 
    int c = 0;
    for(int i = 0; i < n; i++)
    {
        c = a + b;
        a = b;
        b = c;
    }
    return c;
    
}

int recursiveFibo(int n)
{
    if( n < 2)
    {
        return 1;
    }
    
    return recursiveFibo(n-1) + recursiveFibo(n-2);
    
}



int main()
{
    cout << "Brute Fibo = " << fibo(6) << "\n";
    cout << "Brute Fibo = " << fibo(5) << "\n";
    
    cout << "Recursive Fibo = " << recursiveFibo(6) << "\n";
    cout << "Recursive Fibo = " << recursiveFibo(5) << "\n";
    
 
    
    return 0;
}