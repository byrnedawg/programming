#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;
typedef double (*FuncOneVar)(double);

double f(double x) {
	return (13 - x*x - (2*x));
	//return (9 - x*x);
}

double k(double x) {
	return (9 - x*x);
}

const double PHI = (1 + sqrt(5)) / 2;
const int percision = 5;

double goldenMean(FuncOneVar f, double L, double R, double eps) {
  double S = (R-L) / PHI;
	double a = R - S;
	double b = L + S;

	double ya = f(a);
	double yb = f(b);
	cout << "phi=" << PHI << '\n';
		cout << "Left\t\t Right\t\t S\t\t a\t\t b\t \n";
		cout << setprecision(percision) <<
			L << "\t" << R << "\t" << S << "\t" << a << "\t" << b << '\n';
	do  {
		if (ya > yb) {
			R = b;
			b = a;
			S = (R - L) / PHI;
			a = R - S;
			ya = f(a);
			yb = f(b);
		} else {
			L = a;
			a = b;
			S = (R - L) / PHI;
			b = L + S;
			ya = f(a);
			yb = f(b);
		}
		cout << setprecision(percision) <<
			L << "\t" << R << "\t" << S << "\t" << a << "\t" << b << '\n';
	} while (fabs(b-a) > eps );
	cout << setprecision(percision) <<
			a << "\t" << b << "\t" << '\n';
			return a;
}


int main() {
	double golden = goldenMean(f, -3, 5, .1);
	cout << "Max x,y for the function is = (" << golden << ", " << f(golden) <<") " << '\n';
}