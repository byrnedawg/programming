// Name: Gregory Byrne
// Assignment: HW1f Mystery Sum


#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    float sum1 = 0.0f;
    float sum2 = 0.0f;
    int n = 53878; // n = 4096 sum1 max out 53878 sum 2 max out
    int j = n;
    
    for(int i = 1; i<= j; i++)
    {
        sum1 += 1.0f / pow(i, 2);
        sum2 += 1.0f / pow(n, 2);
        n--;
        // cout << sum << endl;
    }
    
    cout << "Float forward Sum1 = " << sum1 << endl;
    cout << "Float backward sum2 = " << sum2 << endl;
    cout << "with n = 4096 sum1 max out and n = 53878 sum 2 max out" << endl;
    cout << "Sqrt of 6*sum1 = " << sqrt(6.0f*sum1) << endl;
    cout << "Sqrt of 6*sum2 = " << sqrt(6.0f*sum2) << endl;
    cout << "Sum2 is more accurate!!!" << endl;
    
    return 0;
}