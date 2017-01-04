#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Dog.h"

Dog::Dog(int height, int weight, int age, string name, string sound) :
	Animal(height, weight, age, name){
		this->sound = sound;
}

void Dog::toString(){
	cout << this->getName() << " is " << this->getHeight() << " cms tall,  " <<
		this->getWeight() << " kgs in weight, " << this->getAge() << " years old and says " <<
		this->sound << endl;
}