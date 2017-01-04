// Name: Gregory Byrne
// Assignment: HW2 - Fermat
#include <iostream>
#include <fstream>
#include <string>
#include <random>

using namespace std;


int powerMod(unsigned long long x, int n, int m){  // O(log n)
    
    int prod = 1;
    while(n > 0){
        if(n % 2 != 0){
            prod = (prod * x) % m;
        }
        n = n / 2;
        x = (x * x) % m;
    }
    return prod;
}

bool fermat(long p, int k)
{
    for(int i = 1; i <= k; i++)
    {
        int a = rand() % (p - 1) + 2; // 2 - p-1
        
        if((powerMod(a, p-1, p)) != 1)
        {
            return false;
        }
    }
    return true;
    
}

int main () 
{
    unsigned long long testNumber;
    int kFactor = 20;
    ifstream myfile ("hw2.dat");
    if (myfile.is_open())
    {
      while  (myfile >> testNumber)
      {
          cout <<  testNumber << "\t";
          if(fermat(testNumber, kFactor))
          {
            cout << "True\n";
          }
          else
          {
            cout << "False\n";
          }
      }
      myfile.close();
    }
    else 
    {
      cout << "Unable to open file"; 
    }

  return 0;
}