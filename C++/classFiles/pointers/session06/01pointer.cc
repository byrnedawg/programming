#include <iostream>
using namespace std;

int main() {
	int x[10] = {0};
	int* p = x + 2;
	*p = 2;
	cout << x[0] << p[-1]  << p[0] << '\n';   // *(p-1)  *p = *(p+0) 002
	++p;
	p++;
	*++p = 6; // first add one int to the address of p, then write 6 to that loc
	++*p; // look at what p is pointing to and add 1
	for (int i = 0; i < 10; i++)
		cout << x[i] << ' '; // 0 0 2 0 0 7 0 0 0 0 
	cout << '\n';
	p = x;
	*p++ = 4;
	*p++ = 4;
	(*p)++;
	p = &x[9];
	*p = x[5];
	for (int i = 0; i < 10; i++)
		cout << x[i] << ' '; // 4 4 3 0 0 7 0 0 0 7 
	cout << '\n';
	int e = 3;
	int* t = &e;
	cout << *t << '\n';


	int f = 5;
//	p++; //order not guaranteed
	p = &f;
	cout << *p << '\n'; ; // want to print 5
	cout << ++*p << '\n'; ; // want to print 6
	
	int y[] = {5, 4, 2, 1, 6};  // x[0] = 5
	for (p = y; p <= y + 3; p++){ // 5 4 2
		cout << ++*p << ' '; // 6 5 3 
	}
	cout << '\n';
	for (p--; p >= y; p--)
		cout << *p << ' '; // 3 5 6
	cout << '\n';	
	
	int g[] = {1, 2, 3, 4, 5};  // x[0] = 1
	for (p = g; p <= g + 3; p++){ // 1 2 3 4 
		cout << --*p << ' '; // 0 1 2 3 
	}
	cout << '\n';
	for (p--; p >= g; p--) // 3 2 1 0
		cout << ++*p << ' '; // 4 3 2 1 
	
}
	
