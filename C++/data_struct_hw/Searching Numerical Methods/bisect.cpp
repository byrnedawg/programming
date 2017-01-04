#include <cmath>
#include <iostream>
#include <iomanip>
using namespace std;

double f(double x) 
{ 
	//return (x+3) * (x-3);
//	return 9 - x*x;
	return (13 - x*x - (2*x));
}
typedef double  (*FuncOneVar)(double);

double bisection(FuncOneVar f, double a, double b, double eps) {
	double y1 = f(a);
	double y2 = f(b);
	if (y1*y2 > 0) {
		cout << "error?\n";
		return 0;
	}
	if (y1 > y2) {
		swap(a, b);
	}
	double mid;
	do {
		mid = (a + b) / 2;
		double y = f(mid);
		if (y > 0)
			b = mid;
		else if (y < 0)
			a = mid;
		else
			return mid;
	} while (fabs(b - a) > eps );
	return mid;
}



int main() {
	for (double eps = .1; eps > 1e-25; eps *= .1) {
		cout << setprecision(25) << bisection(f, 1, 6, eps) << '\n';
	//	cout << bisection(sin, pi, 3*pi, 0.001) << '\n';
	}
}