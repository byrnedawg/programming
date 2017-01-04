#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Animal.h"

using namespace std;

class Dog: public Animal {
	private:
		string sound;

	public:
		void getClass(){
			cout << "I'm a Dog" << endl;
		}
		void makeSound(){
			cout << "The Dog says " << this->sound << endl;
		}

		Dog() : Animal() {};
		Dog(int, int, int, string, string);

		void toString();
};