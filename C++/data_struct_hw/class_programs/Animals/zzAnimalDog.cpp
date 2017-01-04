//
// Created by hirsch on 22/04/16.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Animal{
		public:
			void getFamily() {
				cout << "We are animals" << endl;
			}

			virtual void getClass(){
				cout << "I'm an animal" << endl;
			}
};

class Dog: public Animal {
	public:
		void getClass(){
			cout << "I'm a Dog" << endl;
		}
};

class GermanShepard: public Dog{
	public:
		void getClass(){
			cout << "I'm a German Shepard" << endl;
		}
		void getDerived(){
			cout << "I'm an Animal and a Dog" << endl;
		}
};

void whatClassAreYou(Animal *ap){
	ap->getClass();
}

int main() {

	// Animal *animal = new Animal;
	// Dog *dog = new Dog;
	// GermanShepard *germanShepard = new GermanShepard;

	// animal->getClass();
	// dog->getClass();
	// germanShepard->getClass();

	// whatClassAreYou(animal);
	// whatClassAreYou(dog);
	// whatClassAreYou(germanShepard);

	// Dog balou;
	// GermanShepard max;
	// Animal *ptrDog = &balou;
	// Animal *ptrGs = &max;

	// ptrDog->getClass();
	// ptrGs->getClass();

	//No method of animal
	//ptrGs->getDerived();

	return 0;
}
