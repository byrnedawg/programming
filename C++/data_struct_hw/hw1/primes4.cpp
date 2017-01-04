//============================================================================
// Name        : jett53.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include<iostream>
#include<math.h>
#include<string.h>
//#include<stdlib.h>
#include <vector>

using namespace std;

// This functions finds all primes smaller than 'limit'
// using simple sieve of eratosthenes. It also stores
// found primes in vector prime[]
void simpleSieve(long a, long limit, vector<int> &prime)
{
    // Create a boolean array "mark[0..n-1]" and initialize
    // all entries of it as true. A value in mark[p] will
    // finally be false if 'p' is Not a prime, else true.
    bool mark[limit+1];
    memset(mark, true, sizeof(mark));

    for (long p=2; p*p<limit; p++)
    {
        // If p is not changed, then it is a prime
        if (mark[p] == true)
        {
            // Update all multiples of p
            for (long i=p*2; i<limit; i+=p)
                mark[i] = false;
        }
    }

    // Print all prime numbers and store them in prime
    for (long p=2; p<limit; p++)
    {
        if (mark[p] == true)
        {
            
            prime.push_back(p);
            if(p >= a)
            {
               
                //cout << p << "  ";
            }
        }
    }
}

// Prints all prime numbers smaller than 'n'
void segmentedSieve(long a, long n, long& prime_count)
{

	// Compute all primes smaller than or equal
    // to square root of n using simple sieve
    long limit = floor(sqrt(n))+1;
    vector<int> prime;
    simpleSieve(a, limit, prime);

    // Divide the range [0..n-1] in different segments
    // We have chosen segment size as sqrt(n).
    long low  = limit;
    long high = 2*limit;

    // While all segments of range [0..n-1] are not processed,
    // process one segment at a time
    while (low < n)
    {
        // To mark primes in current range. A value in mark[i]
        // will finally be false if 'i-low' is Not a prime,
        // else true.
        bool mark[limit+1];
        memset(mark, true, sizeof(mark));

        // Use the found primes by simpleSieve() to find
        // primes in current range
        for (long i = 0; i < prime.size(); i++)
        {
            // Find the minimum number in [low..high] that is
            // a multiple of prime[i] (divisible by prime[i])
            // For example, if low is 31 and prime[i] is 3,
            // we start with 33.
           
            int loLim = floor(low/prime[i]) * prime[i];
            if (loLim < low)
                loLim += prime[i];

            /*  Mark multiples of prime[i] in [low..high]:
                We are marking j - low for j, i.e. each number
                in range [low, high] is mapped to [0, high-low]
                so if range is [50, 100]  marking 50 corresponds
                to marking 0, marking 51 corresponds to 1 and
                so on. In this way we need to allocate space only
                for range  */
            for (int j=loLim; j<=high; j+=prime[i])
                mark[j-low] = false;
        }

        // Numbers which are not marked as false are prime
        for (long i = low; i<=high; i++)
            if (mark[i - low] == true)
            {
                
                 if(i >= a)
                {
                   // cout << i << "  ";
        	        prime_count++;
                }
            }

        // Update low and high for next segment
        low  = low + limit;
        high = high + limit;
        if (high >= n) high = n;
    }
    
        for (long i = 0; i < prime.size(); i++)
        {
            if(prime[i] >= a)
            {
                prime_count++;
            }
        }
       // prime_count+=prime.size();
        
}

// Driver program to test above function
int main()
{
    long primenum = 0;
    long a = 0;
	long n = 0;
	cout << "Enter a range to check for primes \n";
    cout << "Enter Start number \n";
	cin >> a;
	cout << "Enter Stop number \n"; // 104729 is the 10,000th prime
	cin >> n;
    cout << "Primes between " << a << " and " << n << ":\n";
    segmentedSieve(a, n, primenum);
    cout << "The number of primes is = " << primenum  << "\n";
    return 0;
}
