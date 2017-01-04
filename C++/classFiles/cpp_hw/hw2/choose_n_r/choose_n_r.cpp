#include <iostream>
using namespace std;

unsigned int x;
unsigned int y;

double choose(unsigned int n, unsigned int r)
{
    double  fact_n = n;
    double  fact_r = r;
    unsigned int u = n - r;
    double fact_u = u;
    double choose_calc ;
    
    while(n > 1)
    {
        fact_n *=  n-1; 
        n--;
        //cout << fact_n << " "<<endl;
    }
     cout << "fact n = "<< fact_n <<endl;
    while(r > 1)
    {
        fact_r *=  r-1; 
        r--;
        //cout << fact_r << " "<<endl;
    }
    cout << "fact r = "<< fact_r <<endl;
    while(u > 1)
    {
        fact_u *=  u-1; 
        u--;
       // cout << fact_u << " "<<endl;
    }
    cout << "fact u = "<< fact_u <<endl;
    
    choose_calc = fact_n / (fact_r*fact_u);
    return choose_calc;
}

int main() 
{
    cout << "Enter in number for n " << endl;
    cin >> x;
    cout << "Enter in number for r " << endl;
    cin >> y;
    cout << "Function choose returns value = " << choose(x,y) << endl;
  
    return 0;
}
