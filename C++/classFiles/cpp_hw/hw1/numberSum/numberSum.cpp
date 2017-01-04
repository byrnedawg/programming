// Name: Gregory Byrne
// Assignment: HW1c sum Numbers

#include <iostream>

using namespace std;

int main()
{
    int sum = 0;
    int gauss = 0;
    int n = 0;
    
    for(int i = 0; i<=100; i++)
    {
        sum += i;
    }
    n = 100;
    gauss = n*(n+1)/2;
    
    cout << "The sum of the numbers from 1 o 100 is = " << sum << endl;
    cout << "The output of the Gauss formula is = " << gauss << endl;
    
    return 0;
}