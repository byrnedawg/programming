#include <iostream>
#include <cmath>

using namespace std;

int x = 0;
int y = 0;
int z = 0;


int main() 
{
   cout << "Enter value for x " << endl;
   cin >> x;
   cout << "Enter value for y " << endl;
   cin >> y;
   z = pow(x,y);
   cout << x << " to the power of "<< y << " is equal to = " << z << endl;
   return 0;
}
