// Name: Gregory Byrne
// Assignment: HW2c - Multiple primes
#include <iostream>
#include <cmath>
using namespace std;



int main()
{
     int p=0, s;
     const long long prime_size = 1000000;
     long long prime_count = 0;
     int myPrimes[prime_size];
     long long x, n;
     bool prime_array[prime_size] = {true};
     bool primesNow[prime_size] = {true};
     
	 for (int i = 0; i <= prime_size; i++)
	 {
		 prime_array[i] = 1;
	 }
	 
	 cout << "\n";

    cout << "Enter a range to check for primes \n";
    cout << "Enter Start number \n";
    cin >> x;
    cout << "Enter Stop number \n"; // 104729 is the 10,000th prime
    cin >> n;
    
    if(n <= prime_size)
    {
        prime_array[0] = 0;
	    prime_array[1] = 0;
    
    
        long square_root_n = floor(sqrt(n));
        for(long long j = 4; j <= n; j +=2)
        {
            prime_array[j] = 0;
          //  prime_array[(int)(j-x)] = 0;
           // cout << prime_array[j] << " ";
        }
        
        
        for(long long i = 3; i <= square_root_n; i++)
        {
            if(prime_array[i])
           // if(prime_array[(int)(i-x)])
            {
               
                for(long long j = i*i ; j <= n; j += i * 2)
                {
                    prime_array[j] = 0;
                    //prime_array[(int)(j-x)] = 0;
                }
            }
            
        }
       
       
       cout << "\n";
       
        //for(long long i = x; i <= n; i++)
        for(long long i = x; i <= n; i++)
        {
            if(prime_array[i])
            {
                cout << i << " ";
                prime_count++;
            }
        }
       
       cout<< "The Primes in that range is = " << prime_count << "\n"; //
    }
    else
    {
        prime_array[0] = 0;
	    prime_array[1] = 0;
    
    
        long square_root_max = floor(sqrt(prime_size));
        for(long long j = 4; j <= prime_size; j +=2)
        {
            prime_array[j] = 0;
          //  prime_array[(int)(j-x)] = 0;
           // cout << prime_array[j] << " ";
        }
        
        
        for(long long i = 3; i <= square_root_max; i++)
        {
            if(prime_array[i])
            {
               
                for(long long j = i*i ; j <= prime_size; j += i * 2)
                {
                    prime_array[j] = 0;
                }
            }
            
        }
       
       
       cout << "\n";
       
        //for(long long i = x; i <= n; i++)
        for(long long i = 0; i <= prime_size; i++)
        {
            if(prime_array[i])
            {
                //cout << i << " ";
                myPrimes[prime_count++]=i;
            }
        }
       
       //cout<< "The Primes in that range is = " << prime_count << "\n"; //
    
        for(int i=0;i<prime_count;i++)  //for each prime in sqrt(N) we need to use it in the segmented sieve process
        {
            p=myPrimes[i]; //store the prime
            s=x/p;
            s=s*p; //the closest number less than M that is a composite number for this prime p

            for(int j=s;j<=n;j=j+p)
            {
                if(j<x) //because composite numbers less than M are of no concern to use.
                primesNow[(int)(j-x)]=false;//j-M = index in the array primesNow,this is because max index allowed in the array
                    //is not N ,it is DIFF_SIZE so we are storing the numbers offset from M
                    //while printing we will add M and print to get the actual number
            }
        
        }

        for(int i=0;i<prime_count;i++)     //in the above loop the first prime numbers for example say 2,3 are also set to false
        {                          //hence we need to print them in case they  are within range.
            if(myPrimes[i]>=x && myPrimes[i]<=n) //without this loop you will see that for an range(1,30), 2 and 3 does
                cout<<myPrimes[i]<<endl;         //not get printed
        }
    
        for(int i=0;i<n-x+1;++i) // primesNow[]=false for all composite numbers,so prime numbers can be found by checking with true
        {
            if(primesNow[i] == true && (i+x)!=1) //i+M != 1 to ensure that for i=0 and M=1 , 1 is not considered a prime number
            cout<<i+x<<endl; //print our prime numbers in the range
        }   
    
    }
     return 0;
}