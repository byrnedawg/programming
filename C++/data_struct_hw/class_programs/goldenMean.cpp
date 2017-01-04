#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;
typedef double (*FuncOneVar)(double);

double f(double x) {
	return 9 - x*x;
}

const double PHI = (1 + sqrt(5)) / 2;

double goldenMean(FuncOneVar f, double L, double R, double eps) {
  double S = (R-L) / PHI;
	double a = R - S;
	double b = L + S;

	double ya = f(a);
	double yb = f(b);
	cout << "phi=" << PHI << '\n';
	do  {
		if (ya > yb) {
			R = b;
			b = a;
			S = (R - L) / PHI;
			a = R - S;
			ya = f(a);
		} else {
			L = a;
			a = b;
			S = (R - L) / PHI;
			b = L + S;
			yb = f(b);
		}
		cout << setprecision(10) <<
			L << "\t" << R << "\t" << S << "\t" << a << "\t" << b << '\n';
	} while (fabs(b-a) > eps );
}


int main() {
	cout << goldenMean(f, -1, 5, .0001) << '\n';
}