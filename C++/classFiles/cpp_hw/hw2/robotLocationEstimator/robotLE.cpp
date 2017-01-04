// Name: Gregory Byrne
//HW1b Convergence of n-gons
#include <iostream>
#include <cmath>
using namespace std;

const double Pi = 3.14159265; // value of pi
const double sensitivity = 0.00005; //value to force sin and cos to 0

void calcDisplacement(double theta, double distance, double& x, double& y)
{
    theta *= Pi/180; // convert to radians 
    double angleCos = cos(theta); //store cos value
    double angleSin = sin(theta); // store sin value
    if(abs(angleCos) < sensitivity) // check if cos should be 0
    {
        angleCos = 0;
    }
    if(abs(angleSin) < sensitivity) //check if sin should be 0
    {
        angleSin = 0;
    }
    y += distance*angleCos; // modify y location by distance * cos of theta
    x += distance*angleSin; // modify x location by distance * sin of theta
}

int main ()
{
    double x = 0.0; //store x location
    double y = 0.0; // store y location
    double theta = 0; // store angle of heading
    double distance = 0; // store distance traveled in heading
    cout << "The Robot is at Location: x = 0, y =0" << endl; // start location
    while(distance >=0 && theta >= 0) // while no inputs are negative
    {
        cout << "Enter in the angle of heading in degrees = "; // enter heading
        cin >> theta;
        cout << "\nEnter the distance traveled in that heading = "; //enter distance
        cin >> distance;
        cout << endl;
        if(distance >=0 && theta >= 0) //if entered values are not negative
        {
            calcDisplacement(theta, distance, x, y); // call to function pass location x and y by reference
            cout << "Location: x = "<< x <<", y = " << y <<endl;//output location
        }
    }
    //output final location after negative inputs... Note negative inputs ignored and dont effect final
    cout << "The Final location is Location: x = "<< x <<", y = " << y <<endl;
    return 0;
}
