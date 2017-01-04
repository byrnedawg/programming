// Name: Gregory Byrne
// Assignment: HW2c - Multiple primes
#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    double clocking;
    short choice;
    cout << "Enter 1 if you want to determine clocking angle in degrees" << endl;
    cout << "Else enter any other number to determine clock position" << endl;
    cin >> choice;
    if(choice == 1)
    {
        cout << "Please enter the clocking desired off of 12 O' Clock " << endl;
        cin >> clocking;
        cout << "The angle from 12 O' Clock is = " << clocking * 30 << " degrees " <<endl;
    }
    else
    {
        cout << "Please enter the clocking desired angle from the top " << endl;
        cin >> clocking;
        cout << "The  Clock position is = " << clocking / 30 << " O' Clock " <<endl;
    }
    return 0;
}