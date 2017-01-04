#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Animal.h"

using namespace std;

class Cat: public Animal {
	private:
		string sound;

	public:
		void getClass(){
			cout << "I'm a Cat" << endl;
		}
		void makeSound(){
			cout << "The Dog says " << this->sound << endl;
		}

		Cat() : Animal() {};
		Cat(int, int, int, string, string);

		void toString();
};