#include <iostream>
#include <fstream>
#include <random>
using namespace std;

mt19937 randomGen;
static const int n = 52;

class Distribution;

class Deck {
private:
	int cards[n];
public:
	Deck() {
    for (int i = 0; i < n; i++)
			cards[i] = i+1;
	}
#if 0
	void swap(uint32_t& a, uint32_t& b) {
		a = a + b;
		b = a - b;
		a = a - b;
	}
	void swap(uint32_t& a, uint32_t& b) {
		a = a ^ b;
		b = a ^ b;
		a = a ^ b;
	}
	void swap(uint32_t& a, uint32_t& b) {
		uint32_t temp = a;
		a = b;
		b = temp;
	}
#endif

	void FischerYatesShuffle() {
		for (int i = n-1; i >= 0; i--) {
			uniform_int_distribution<uint32_t> pickCard(0,i);
      uint32_t pick = pickCard(randomGen);
			swap(cards[pick], cards[i]);
		}
	}

	void badShuffle() {
		for (int i = n-1; i >= 0; i--) {
			uniform_int_distribution<uint32_t> pickCard(0,n-1);
      uint32_t pick = pickCard(randomGen);
			swap(cards[pick], cards[i]);
		}
	}

	friend ostream& operator <<(ostream& s, const Deck& d) {
		for (int i = 0; i < n; i++)
			s << d.cards[i] << '\t';
		return s << '\n';
	}
	friend Distribution;
};

class Distribution {
private:
	int counts[n][n];
public:
	Distribution() {
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				counts[i][j] = 0;
	}

 	void stats(const Deck& d1) {
		for (int i = 0; i < n; i++)
			counts[i][d1.cards[i]]++;
	}
	void write() {
		ofstream file("stats.dat");
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				file << counts[i][j] << '\t';
			file << '\n';
		}
	}

};

void testUnfairShuffle(int trials) { // O( n*trials   )
	Distribution dist;
	for (int i = 0; i < trials; i++) {
		Deck d1;                             // O(           )
		//		d1.FischerYatesShuffle();            // O(           )
		d1.badShuffle();
		dist.stats(d1);                      // O(           )
	}
	dist.write();
}

int main() {
	Deck d1;
	d1.FischerYatesShuffle();
	cout << d1;
	testUnfairShuffle(100000);
}