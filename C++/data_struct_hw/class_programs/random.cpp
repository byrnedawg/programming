#include <iostream>
#include <random>
using namespace std;

default_random_engine e1;
uniform_int_distribution<int> die(1, 6);
normal_distribution<> N(5, 1);

void randomize(int a[], int n) {
	for (int i = 0; i < n; i++)
		a[i] = rand();
}

int main() {
	//	int a[1000000];
	//	randomize(a, sizeof(a)/sizeof(int));
	for (int i = 0; i < 100; i++) {
		int x = die(e1);
		cout << x << ' ';
	}
	cout << '\n';
	for (int i = 0; i < 100; i++) {
		cout << N(e1) << ' ';
	}
	cout << '\n';
}