
#include<iostream>
#include<math.h>
#include<string.h>
//#include<stdlib.h>
#include <vector>

using namespace std;


int powermod(int a, int n, int m){
    
    int prod = 1;
    while(n > 0){
        if(n % 2 != 0){
            prod = (prod * a) % m;
        }
        n = (int)(n / 2);
        a = (a * a) % m;
    }
    return prod;
}

int main()
{
     
     cout << " The power mod result is = " << powermod(2, 16, 37) << "\n";

    return 0;
}