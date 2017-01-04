#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Cat.h"

Cat::Cat(int height, int weight, int age, string name, string sound) :
	Animal(height, weight, age, name){
		this->sound = sound;
}

void Cat::toString(){
	cout << this->getName() << " is " << this->getHeight() << " cms tall,  " <<
		this->getWeight() << " kgs in weight, " << this->getAge() << " years old and says " <<
		this->sound << endl;
}