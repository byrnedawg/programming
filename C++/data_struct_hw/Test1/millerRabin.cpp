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
       unsigned long long a = rand() % (orig - 2) + 2; // -2 to orig - 2
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
  ifstream myfile ("hw2.dat");
  if (myfile.is_open()){
    while  (myfile >> testNumber){
      cout <<  testNumber << "\t";
      if(millerRabin(testNumber)){
        cout << "True\n";
      }
      else cout << "False\n";
    }
    myfile.close();
  }

  else cout << "Unable to open file"; 

  return 0;
}