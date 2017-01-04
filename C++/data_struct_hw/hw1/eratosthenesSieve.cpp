// Name: Gregory Byrne
// Assignment: HW1 - Eratosthenes Sieve
#include<iostream>
#include<math.h>
#include<string.h>
//#include<stdlib.h>
#include <vector>

using namespace std;

// This functions finds all primes smaller than sqrt of n
void simpleSieve(unsigned long long a, unsigned long long limit, vector<unsigned long long> &prime)
{
    // Create a boolean array sieveArray and initialize true
    bool sieveArray[limit+1];
    memset(sieveArray, true, sizeof(sieveArray));


    sieveArray[1] = false; // value for 0
    sieveArray[2] = true; // value for 1

    for (unsigned long long p=4; p<limit; p+=2)
    {
        // If p is not changed, then it is a prime
        sieveArray[p] = false;
    }

    for(unsigned long long i = 3; i <= limit; i++)
     {
            if(sieveArray[i])
            {

                for(unsigned long long j = i*i ; j <= limit; j += i * 2)
                {
                    sieveArray[j] = false;

                }
            }

        }


    // Print all prime numbers and store them in prime
    for (unsigned long long p=2; p<limit; p++)
    {
        if (sieveArray[p] == true)
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
void segmentedSieve(unsigned long long a, unsigned long long n, unsigned long long& prime_count)
{

	// Compute all primes smaller than or equal
    // to square root of n using simple sieve
    unsigned long long limit = floor(sqrt(n))+1;
    vector<unsigned long long> prime;
    simpleSieve(a, limit, prime);

    // Divide the range [0..n-1] in different segments
    // We have chosen segment size as sqrt(n).
    unsigned long long low  = limit;
    unsigned long long high = 2*limit;

  
    while (low < n)
    {

        // false if i-low is not a prime else true

        bool sieveArray[limit+1];
        memset(sieveArray, true, sizeof(sieveArray));

        // Use the found primes by simpleSieve() to find
        // primes in current range
        for (unsigned long long i = 0; i < prime.size(); i++)
        {
            // Find the minimum number in [low..high] that is
            // a multiple of prime[i] (divisible by prime[i])

            unsigned long long lowLim = floor(low/prime[i]) * prime[i];
            if (lowLim < low){
                lowLim += prime[i];
            }

           
            for (unsigned long long j=lowLim; j<=high; j+=prime[i]){
                
                sieveArray[j-low] = false;
            }
        }

        // Numbers which are not marked as false are prime
        for (unsigned long long i = low; i<=high; i++)
            if (sieveArray[i - low] == true)
            {

                 if(i >= a)
                {
                    //cout << i << "  ";
        	        prime_count++;
                }
            }

        // Update low and high for next segment
        low  = low + limit;
        high = high + limit;
        if (high >= n)
        {
        	high = n;
        }
    }

        for (unsigned long long i = 0; i < prime.size(); i++)
        {
            if(prime[i] >= a)
            {
                prime_count++;
            }
        }
       // prime_count+=prime.size();

}

// main
int main()
{
	bool answer = true;

	do{
		unsigned long long primenum = 0;
		unsigned long long a = 0;
		unsigned long long n = 0;
		cout << "Enter a range to check for primes \n";
		cout << "Enter Start number \n";
		cin >> a;
		cout << "Enter Stop number \n"; // 104729 is the 10,000th prime
		cin >> n;
		cout << "Primes between " << a << " and " << n << ":\n";
		segmentedSieve(a, n, primenum);
		cout << "The number of primes is = " << primenum  << "\n";
		cout<< "Run the program again 1 = Yes or 0 = No? \n";
		cin >> answer;
	}while(answer);

	return 0;
}
