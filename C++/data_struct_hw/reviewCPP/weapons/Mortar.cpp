#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Mortar.h"

Mortar::Mortar(int height, int weight, int age, string name, string sound) :
	Weapon(height, weight, age, name){
		this->sound = sound;
}

void Mortar::toString(){
	cout << this->getName() << " is " << this->getHeight() << " cms tall,  " <<
		this->getWeight() << " pounds in weight, " << this->getAge() << " years old and says " <<
		this->sound << endl;
}