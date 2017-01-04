#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Animal.h"

int Animal::numOfAnimals = 0;

Animal::Animal(int height, int weight, int age, string name){
	this->height = height;
	this->weight = weight;
	this->age = age;
	this->name = name;
	Animal::numOfAnimals++;
}

Animal::~Animal(){
	cout << "Animal " << this->name << " destroyed" << endl;
	Animal::numOfAnimals--;
}

Animal::Animal(){
	Animal::numOfAnimals++;
}

Animal::Animal(string s){
	char delemiter = ',';
	string tmp;
	int step = 0;
	for (int i = 0; i < s.length(); i++){
		if(s[i] == delemiter){
			switch (step){
				case 0:
					this->name = tmp;
					break;
				case 1:
					this->age = stoi(tmp);
					break;
				case 2:
					this->weight = stoi(tmp);
					break;
				case 3:
					this->height = stoi(tmp);
					break;
			}
			step++;
			tmp = "";
		}else{
			tmp += s[i];
		}
	}
}

void Animal::toString(){
	cout << this->name << " is " << this->height << " cms tall,  " << this->weight << " kgs in weight and " <<
		this->age << " years old" << endl;
}

bool Animal::compareTo(Animal& animal){
	Animal* ap = &animal;
	return this->age == ap->getAge();
}

bool Animal::compareTo(Animal* ap){
	return this->age == ap->getAge();
}

Animal& Animal::operator++(){
	this->age++;
	return *this;
}

Animal& Animal::operator--(){
	this->age--;
	return *this;
}

Animal& Animal::operator++(int){
	this->age++;
	return *this;
}

Animal& Animal::operator--(int){
	this->age--;
	return *this;
}

bool Animal::operator==(const Animal& animal){
	return this->age == animal.age;
}

string Animal::toJson(){
	return "{\"name\": \"" + this->name +  "\", " +
	"\"age\": \"" + to_string(this->age) +  "\", " +
	"\"weight\": \"" + to_string(this->weight) +  "\", " +
	"\"height\": \"" + to_string(this->height) +
	"\"}";
}

string Animal::toStringArray(){
	return this->name + "," +
		to_string(this->age) + "," +
		to_string(this->weight) + "," +
		to_string(this->height);
}