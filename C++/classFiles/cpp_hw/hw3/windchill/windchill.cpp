#include <iostream>
#include <math.h>

using namespace std;

int main() 
{
   int wind_speed = 0;
   int air_temp = 0;
   double wind_chill = 0.0;
   cout << "Enter value for wind speed  " << endl;
   cin >> wind_speed;
   cout << "Enter value for air temp " << endl;
   cin >> air_temp;
   wind_chill = 35.74 + (0.6215*air_temp) - (35.75*pow(wind_speed, 0.16)) + (0.4275* air_temp*pow(wind_speed, 0.16));
   cout << "The Wind Chill calculated is = "<< wind_chill << endl;
   cout << "Int Wind Chill is = "<< round(wind_chill) << endl;
   return 0;
}
