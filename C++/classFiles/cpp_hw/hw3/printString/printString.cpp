// Name: Gregory Byrne
// Assignment: HW3g - print string
#include <iostream>
#include <string>

using namespace std;

// fill in s with the  letters of the decimal value of v
// ex. if v = 345   '3' '4'  '5'
// return the length of the number in characters
// ex. -9876 returns 5
/// biggest +/- 2100000000
//int toString(char s[], int v) {
int toString(char s[], long v) {

    int i, sign;
    int length = 0;

    if ((sign = v) < 0)  // record sign 
        v = -v;          // make v positive 
    i = 0;
  do {       
       s[i] = v % 10 + '0';  // get next digit 
       i++;
   } while ((v /= 10) > 0); // // last digit
   
    if (sign < 0) //if its a negative number
    {
        s[i] = '-'; // add sign character
        i++;
    }
    s[i] = '\0'; // null character
    length = i; // store length value
    for(i; i>= 0; i--) // read out array in backwards order 
    {
         cout << s[i] << ' '; // print array of chars with a space in between
    }
    cout << "The Length of the string is = " ;
    
    return length;
}

//  return length of s 
int lengthString(char s[]){
    int i = 0;
    while (s[i] != '\0')
        ++i;
    return i;
}

int main() 
{
   // char runagain = 'y';
    char data[11];
    long value;
    int lenght = 0;
   // do{
        cout << "Enter in the value to convert to string \n";
        cin >> value;
        cout << toString(data, value) << "\n";
//        cout << "Try another number y or n ? \n";
  //      cin >> runagain;
  //  } while ( runagain == 'y' || runagain == 'Y' );
    return 0;
}
