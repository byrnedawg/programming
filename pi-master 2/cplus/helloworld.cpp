#include <iostream>

using namespace std;

int main()
{
	int x;
	short y;
	bool z;
	long a;
	long long b;

	cout << "Hello World !! Enter in value for x" << endl;
	cin >> x;
	cout << "The size of x is = " << sizeof(x) << endl;
	cout << "The size of y is = " << sizeof(y) << endl;
	cout << "The size of z is = " << sizeof(z) << endl;
	cout << "The size of a is = " << sizeof(a) << endl;
	cout << "The size of b is = " << sizeof(b) << endl;

	cout << "The value entered for x is = " << x << endl;
	return 0;
}
