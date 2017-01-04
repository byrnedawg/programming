// Name: Gregory Byrne
//HW1b Convergence of n-gons
#include <iostream>
#include <cmath>
using namespace std;

const double Pi = 3.14159265; // value of pi


double radius_n_gons(unsigned long ngons)
{
    double radius = 1.0; // intital radius value
    for(int i = 3; i<= ngons; i++) // start at 3 thats where n-gons equation begin 
    {
        radius = 1/cos(Pi/i) * radius; // the equation is a product of values to n
    }
    return radius;
}
int main() 
{
    unsigned long n;   // number of n - gons
   
    cout << "Enter how many polygons are in within the circle n = " ;
    cin >> n; // enter in value of n
   
    // Print out the n and radius values
    cout << "The radius of n = "<< n <<" is radius = " << radius_n_gons(n) << endl;
    cout << "The radius of n = 10 is radius = " << radius_n_gons(10) << endl;
    cout << "The radius of n = 100 is radius = " << radius_n_gons(100) << endl;
    cout << "The radius of n = 1000 is radius = " << radius_n_gons(1000) << endl;
    cout << "The radius of n = 10000 is radius = " << radius_n_gons(10000) << endl;
    cout << "The radius of n = 100000 is radius = " << radius_n_gons(100000) << endl;
    cout << "The radius of n = 1000000 is radius = " << radius_n_gons(1000000) << endl;
    return 0;
}
