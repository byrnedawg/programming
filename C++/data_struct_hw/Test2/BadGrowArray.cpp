#include<iostream>
using namespace std;

class BadGrowArray {
private:
	int* data;
	int size;
public:
	BadGrowArray() {
		data = nullptr; // O(1)
		size = 0;
	}
	BadGrowArray(int initialSize) {
		data = new int[initialSize]; //O(1)
		size = initialSize;
	}
	~BadGrowArray() {
		delete [] data;
	}
	void addEnd(int v) {   //O(n)
		int* temp = data; // O(1)
		
    data = new int[size+1]; // O(1)
		for (int i = 0; i < size; i++) //O(n)
			data[i] = temp[i];
		data[size] = v;
		size++;
	}
	void addStart(int v) { //O(n)
		int* temp = data;
		data = new int[size+1];
		for (int i = 0; i < size; i++) //O(n)
			data[i+1] = temp[i];
		data[0] = v;
		size++;
	}
	void removeEnd() { //O(1)
		size--;
	}
	void removeStart() { //O(n)
		for (int i = 0; i < size-1; i++)
			data[i] = data[i+1];
		size--;
	}
};



int main() {
	BadGrowArray a; // empty list
	//	BadGrowArray b(1000); // has 1000 elements (random)
	for (int i = 0; i < 100; i++)
		a.addEnd(i);
	//a.addStart(3);
}
