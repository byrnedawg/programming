#include <iostream>

using namespace std;


int main()
{
    char hello[] = "hello";
    
    char *f = hello;
    
    while (*f)
    {
        *f += 1; // increase the character by one
    
        f+= 1; // move to the next spot
    }
    
    cout << hello << "\n"; // goes through the array hello and increases the character value by one and then increments the pointer
    
    int x[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const int arraySize = sizeof(x)/sizeof(int);
    int* p = x+9;
    
    cout << "The value in the 10th pointer: " << *p << "\n";
	
	p = x;
	for(int i = 1; i <= arraySize; i++)
	{
    	*p += i;
    	p++;
    	*P += I
	}
   // cout <<"original array {" << x << " }\n";
	//cout << x[0] << p[-1]  << p[0] << '\n';   // *(p-1)  *p = *(p+0) 002
//	++p;
//	p++;
	//*++p = 6; // first add one int to the address of p, then write 6 to that loc
//	++*p; // look at what p is pointing to and add 1
	for (int i = 0; i < arraySize; i++)
		cout << x[i] << ' '; // 0 0 2 0 0 7 0 0 0 0 
	cout << '\n';
	
	return 0;
}