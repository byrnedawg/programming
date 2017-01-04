// Name: Gregory Byrne
// Assignment: HW2c - Multiple primes
#include <iostream>
#include <cmath>
using namespace std;

unsigned long long x;
unsigned long long y;

bool isPrime(unsigned long long n) 
{
	if(n < 2) //check for prime 0 and 1 case
	{
	    return false;
	}
	if (n == 2) // check for prime 2 case
	{
		return true;
	}
	if (n % 2 == 0) // any even number will not be prime other than 2
	{
		return false;
	}
	// start at 3 and go up by odd numbers up to the square root of n to check 
	for (unsigned long long i = 3; i <= sqrt(n); i += 2) 
	{
		if (n % i == 0) // check divisiability of n by i
		{
			return false;
		}
	}
	return true;
}

int countPrimes(unsigned long long a, unsigned long long b)
{
    unsigned int primecount = 0; // initialize prime count to 0
    for(a; a<=b; a++) // start range at a and check till b
    {
        if(isPrime(a))
        {
            primecount++;
        }
    }
    return primecount;
    
}

int main() 
{
    cout << "Enter a range to check for primes" << endl;
    cout << "Enter Start number" << endl;
    cin >> x;
    cout << "Enter Stop number" << endl; // 104729 is the 10,000th prime
    cin >> y;
    cout<< "The Number of Primes in that range is = "<< countPrimes(x,y) <<endl;
    
    return 0;
}
