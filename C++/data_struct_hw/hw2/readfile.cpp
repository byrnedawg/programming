// Name: Gregory Byrne
// Assignment: HW2 - Miller Rabin
#include <iostream>
#include <fstream>
#include <string>
#include <random>

using namespace std;




int powerMod(unsigned long long x, int n, int m){
    
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

bool fermat(long p, int k){
    default_random_engine generator;
    for(int i = 1; i <= k; i++){
        uniform_int_distribution<int> distribution(2,p-1);
        int a = distribution(generator);
        
        if((powerMod(a, p-1, p)) != 1){
            return false;
        }
    }
    return true;
    
}

bool millerRabin(unsigned long long sample)
{
  default_random_engine generator;
  unsigned long long orig = sample;
  sample -= 1;
  int r = 0;
  bool flag = false;
  while(sample % 2 == 0){
      sample = sample / 2;
      r++;
  }
  for(int k = 0; k <= log(orig); k++){
       uniform_int_distribution<unsigned long long> distribution(2,orig-2);
       unsigned long long a = distribution(generator);
       unsigned long long x = powerMod(a, sample, orig);
       if(x == 1 || x == orig - 1) continue;
       for(int i = 0; i < r; i++){
           x = (x*x) % orig;
           if(x == 1) return false;
           if(x == orig -1){
               flag = true;
               break;
           }
       }
       if(flag) continue;
       else return false;
           
       
  }
  return true;
}

int main () {
  unsigned long long testNumber;
  int kFactor = 20;
  ifstream myfile ("hw2.dat");
  if (myfile.is_open())
  {
    while  (myfile >> testNumber)
    {
      cout <<  testNumber << "\t";
      if(millerRabin(testNumber))
      //if(fermat(testNumber, kFactor))
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

  else cout << "Unable to open file"; 

  return 0;
}