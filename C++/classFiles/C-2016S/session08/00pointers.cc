#include <iostream>
using namespace std;

int main() {
	int a = 3;
	int* p = &a;
	cout << *p << '\n';


	int b = 5;
//	p++; //order not guaranteed
	p = &b;
	cout << *p << '\n'; ; // want to print 5



	
	int x[] = {5, 4, 2, 1, 6};  // x[0] = 5
	for (p = x; p < x + 3; p++) // 6 5 3 1 6
		++*p;
	for (p--; p > x; p--)
		cout << *p << '\n';;

}


	
