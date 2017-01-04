// Name: Gregory Byrne
// Assignment: HW2d choose(n,r)
#include <iostream>
using namespace std;

unsigned int x;
unsigned int y;

double choose(unsigned int n, unsigned int r)
{
    double  fact_n = n;
    double  fact_r = r;
    unsigned int u = n - r;
    double fact_diff = u;
    double choose_calc;
    
    for(n; n > 1; n--)
    {
        fact_n *=  n-1; 
        //cout << fact_n << " "<<endl;
    }
    // cout << "fact n = "<< fact_n <<endl;
    for(r; r > 1; r--)
    {
        fact_r *=  r-1; 
        //cout << fact_r << " "<<endl;
    }
   // cout << "fact r = "<< fact_r <<endl;
    for(u; u > 1; u--)
    {
        fact_diff *=  u-1; 
       // cout << fact_u << " "<<endl;
    }
   // cout << "fact u = "<< fact_diff <<endl;
    
    choose_calc = fact_n / (fact_r*fact_diff);
    return choose_calc;
}

int main() 
{
    cout << "Enter in number for n " << endl;
    cin >> x;
    cout << "Enter in number for r " << endl;
    cin >> y;
    cout << "Function choose returns the value = " << choose(x,y) << endl;
  
    return 0;
}
