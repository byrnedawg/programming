//
// Created by hirsch on 22/04/16.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// #include "Animal.h"
#include "Dog.h"
#include "Cat.h"

using namespace std;

// void output(int i){ cout << i << endl; }
// void output(float f){ cout << f << endl; }
// void output(char c){ cout << c << endl; }
// void output(const char* word){ cout << word << endl; }

// instead of class you can use typename
template <class T>
void output(const T & n){
 cout << n << endl;
}

template <class T>
void swapThis(T & a, T & b){
 T tmp = a;
 a = b;
 b = tmp;
}

template <class T>
class A {
	private:
		T data;
	public:
		A(const T& init);
		void output();
};

template <class T>
A<T>::A(const T& init){
	data = init;
}

template <class T>
void A<T>::output(){
	cout << data << endl;
}


int main() {

	// Functions
	output(1);
	output(2.34);
	output('a');
	output("wurst");

	int array[5] = { 1,3,5,7,9 };
	swapThis(array[1], array[3]);
	for(int i=0; i < 5; i++){
		cout << array[i] << endl;
	}

	// Classes
	A<int> number(5);
	A<char> letter('c');
	number.output();
	letter.output();

	// everything went successful
	return 0;
}
