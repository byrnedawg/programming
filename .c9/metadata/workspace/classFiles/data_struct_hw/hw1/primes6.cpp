{"filter":false,"title":"primes6.cpp","tooltip":"/classFiles/data_struct_hw/hw1/primes6.cpp","undoManager":{"mark":1,"position":1,"stack":[[{"start":{"row":0,"column":0},"end":{"row":168,"column":0},"action":"insert","lines":["//============================================================================","// Name        : jett53.cpp","// Author      : ","// Version     :","// Copyright   : Your copyright notice","// Description : Hello World in C++, Ansi-style","//============================================================================","","#include<iostream>","#include<math.h>","#include<string.h>","//#include<stdlib.h>","#include <vector>","","using namespace std;","","// This functions finds all primes smaller than 'limit'","// using simple sieve of eratosthenes. It also stores","// found primes in vector prime[]","void simpleSieve(long long a, long long limit, vector<long> &prime)","{","    // Create a boolean array \"sieveArray[0..n-1]\" and initialize","    // all entries of it as true. A value in sieveArray[p] will","    // finally be false if 'p' is Not a prime, else true.","    bool sieveArray[limit+1];","    memset(sieveArray, true, sizeof(sieveArray));","","","    sieveArray[1] = false; // value for 0","    sieveArray[2] = true; // value for 1","","    for (long p=4; p<limit; p+=2)","    {","        // If p is not changed, then it is a prime","        sieveArray[p] = false;","    }","","    for(long long i = 3; i <= limit; i++)","     {","            if(sieveArray[i])","            {","","                for(long long j = i*i ; j <= limit; j += i * 2)","                {","                    sieveArray[j] = false;","","                }","            }","","        }","","","    // Print all prime numbers and store them in prime","    for (long p=2; p<limit; p++)","    {","        if (sieveArray[p] == true)","        {","","            prime.push_back(p);","            if(p >= a)","            {","","                //cout << p << \"  \";","            }","        }","    }","}","","// Prints all prime numbers smaller than 'n'","void segmentedSieve(long long a, long long n, long& prime_count)","{","","\t// Compute all primes smaller than or equal","    // to square root of n using simple sieve","    long long limit = floor(sqrt(n))+1;","    vector<long> prime;","    simpleSieve(a, limit, prime);","","    // Divide the range [0..n-1] in different segments","    // We have chosen segment size as sqrt(n).","    long long low  = limit;","    long long high = 2*limit;","","    // While all segments of range [0..n-1] are not processed,","    // process one segment at a time","    while (low < n)","    {","","        // false if i-low is not a prime else true","","        bool sieveArray[limit+1];","        memset(sieveArray, true, sizeof(sieveArray));","","        // Use the found primes by simpleSieve() to find","        // primes in current range","        for (long i = 0; i < prime.size(); i++)","        {","            // Find the minimum number in [low..high] that is","            // a multiple of prime[i] (divisible by prime[i])","            // For example, if low is 31 and prime[i] is 3 we start with 33.","","            long loLim = floor(low/prime[i]) * prime[i];","            if (loLim < low)","                loLim += prime[i];","","            /*  sieveArray multiples of prime[i] in [low..high]:","                We are marking j - low for j, i.e. each number","                in range [low, high] is mapped to [0, high-low]","               */","            for (long j=loLim; j<=high; j+=prime[i])","                sieveArray[j-low] = false;","        }","","        // Numbers which are not marked as false are prime","        for (long i = low; i<=high; i++)","            if (sieveArray[i - low] == true)","            {","","                 if(i >= a)","                {","                    //cout << i << \"  \";","        \t        prime_count++;","                }","            }","","        // Update low and high for next segment","        low  = low + limit;","        high = high + limit;","        if (high >= n)","        {","        \thigh = n;","        }","    }","","        for (long i = 0; i < prime.size(); i++)","        {","            if(prime[i] >= a)","            {","                prime_count++;","            }","        }","       // prime_count+=prime.size();","","}","","// main","int main()","{","\tbool answer = true;","","\tdo{","\t\tlong primenum = 0;","\t\tlong long a = 0;","\t\tlong long n = 0;","\t\tcout << \"Enter a range to check for primes \\n\";","\t\tcout << \"Enter Start number \\n\";","\t\tcin >> a;","\t\tcout << \"Enter Stop number \\n\"; // 104729 is the 10,000th prime","\t\tcin >> n;","\t\tcout << \"Primes between \" << a << \" and \" << n << \":\\n\";","\t\tsegmentedSieve(a, n, primenum);","\t\tcout << \"The number of primes is = \" << primenum  << \"\\n\";","\t\tcout<< \"Run the program again 1 = Yes or 0 = No? \\n\";","\t\tcin >> answer;","\t}while(answer);","","\treturn 0;","}",""],"id":1}],[{"start":{"row":0,"column":0},"end":{"row":7,"column":0},"action":"remove","lines":["//============================================================================","// Name        : jett53.cpp","// Author      : ","// Version     :","// Copyright   : Your copyright notice","// Description : Hello World in C++, Ansi-style","//============================================================================",""],"id":2}]]},"ace":{"folds":[],"scrolltop":2572.5,"scrollleft":0,"selection":{"start":{"row":157,"column":8},"end":{"row":157,"column":14},"isBackwards":false},"options":{"guessTabSize":true,"useWrapMode":false,"wrapToView":true},"firstLineState":0},"timestamp":1473734064421,"hash":"db6d3541d9d11165d6628027ed1bc69a94c72145"}