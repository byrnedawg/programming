#include <iostream>
#include<math.h>
#include<string.h>


using namespace std;

void eratosthenes(int n) 
{
   
    bool sieve[n]; //O(n)
    for (int i = 2; i < n; i++)
    {
        sieve[i] = true;
    }
    for (int i = 2; i < n; i++) 
    {
        if (sieve[i]) 
        {
            cout << " " << i << " ";
            for (int j = 2*i; j < n; j += i)
            {
                sieve[j] = false;
            }
        }
    }
}  


void simpleSieve(unsigned long long a, unsigned long long n)
{
    // Create a boolean array sieveArray and initialize true
    bool sieveArray[n];
   // memset(sieveArray, true, sizeof(sieveArray));
   
    for (int i = 2; i < n; i++)
    {
        sieveArray[i] = true;
    }
    
    long limit = sqrt(n);
    sieveArray[1] = false; // value for 0
    sieveArray[2] = true; // value for 1

    for (unsigned long long p=4; p<n; p+=2)
    {
        // If p is not changed, then it is a prime
        sieveArray[p] = false;
    }

    for(unsigned long long i = 3; i <= n; i++)
     {
            if(sieveArray[i])
            {

                for(unsigned long long j = i*i ; j <= n; j += i * 2)
                {
                    sieveArray[j] = false;

                }
            }

        }


    // Print all prime numbers and store them in prime
    for (unsigned long long p=2; p<n; p++)
    {
        if (sieveArray[p] == true)
        {

            if(p >= a)
            {

                cout << p << "  ";
            }
        }
    }
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
		eratosthenes(n);
		cout <<" \n";
		simpleSieve(a, n);
		cout << "The number of primes is = " << primenum  << "\n";
		cout<< "Run the program again 1 = Yes or 0 = No? \n";
		cin >> answer;
	}while(answer);

	return 0;
}
