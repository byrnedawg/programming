// Name: Gregory Byrne
// Assignment: HW3a - Windchill
#include <iostream>
#include <cmath>

using namespace std;

double calc_wind_chill(int wind_speed, int air_temp)
{
   double wind_chill = 0.0;
   // calculating windchill based on wind and air
   wind_chill = 35.74 + (0.6215*air_temp) - (35.75*pow(wind_speed, 0.16)) + (0.4275* air_temp*pow(wind_speed, 0.16));
   if(round(wind_chill) == -0) // check if the round will print -0
   {
      return abs(round(wind_chill)); // get rid of the sign with 0
   }
   return round(wind_chill); // return for all other cases
}
int main() 
{
   short index = 7; // char index for Wind MPH label
   char windtext[] = "Wind MPH"; // chart side label
   cout << " \t \t \t \t \t \t Temperature in Degrees (Degrees F)\n"; // chart top label
   cout << '\t'; // tap space to allign labels with their values
   for(int k = 40; k >= -45; k = k-5) // go through all temps for printing 
      {
         cout << k << '\t'; // print label with tab in between
      }
      cout << endl;
   for(int i = 5; i<=60; i = i+5) // go through all the wind speeds
   {
      if(i > 10 && index >= 0) // location to start the side label
      {
         cout << windtext[index] << ' '; //print out label in reverse
         index--;
      }
      else
      {
         cout << "  "; // space for after the side label is printed
      }
      cout << i << '\t'; // print the wind speed with a tab
      
      for(int j = 40; j >= -45; j = j-5) // go through range of temps
      {
         cout <<calc_wind_chill(i, j) << '\t'; // print out the windchill for wind and temp
      }
      cout << endl;
   }
   return 0;
}
