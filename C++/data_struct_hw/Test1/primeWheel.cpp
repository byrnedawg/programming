// Name: Gregory Byrne

#include <iostream>
#include <cmath>

using namespace std;

bool isPrime(int n) {
  if (n == 2)
    // cout << n << " is prime \n";
     return true;
  if (n % 2 == 0)
    return false;
  for (int i = 3; i <= sqrt(n); i+=2) { //O(n)
     if (n % i == 0)
       return false;
  }
  //cout << n << " is prime \n";
  return true;
}


void testPrime(int n)
{
    for(int i = 6; i <= n; i+=6)
    {
       if(isPrime(i+1))
       {
           cout << i+1 << " is prime \n";
       }
       if(isPrime(i+5))
       {
           cout << i+5 << " is prime \n";
       }
        
    }
}



int main()
{
    testPrime(100);
    
    return 0;
}