// Name: Gregory Byrne
// Assignment: HW1d sum Reciprocals

#include <iostream>

using namespace std; 

int main()
{
    float sum1float = 0.0f;
    float sum2float = 0.0f;
    double sum1double = 0.0;
    double sum2double = 0.0;
    int n = 2000000;
    int j = n;
    
    for(int i = 1; i<= j; i++)
    {
        
        sum1float += (float) 1.0f/i;
        sum2float += (float) 1.0f/n;
        sum1double += (double) 1.0/i;
        sum2double += (double) 1.0/n;
        n--;
    }
    
    cout << "Float Sum 1 = " << sum1float << endl;
    cout << "Float Sum 2 = " << sum2float << endl;
    cout << "Float Sum1 - Sum2 = " << sum1float - sum2float << endl;
    
    cout << "Double Sum 1 = " << sum1double << endl;
    cout << "Double Sum 2 = " << sum2double << endl;
    cout << "Double Sum1 - Sum2 = " << sum1double - sum2double << endl;
    
    
    return 0;
}