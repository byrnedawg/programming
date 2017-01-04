#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Weapon.h"

int Weapon::numOfWeapons = 0;

Weapon::Weapon(int height, int weight, int age, string name){
	this->height = height;
	this->weight = weight;
	this->age = age;
	this->name = name;
	Weapon::numOfWeapons++;
}

Weapon::~Weapon(){
	cout << "Weapon " << this->name << " destroyed" << endl;
	Weapon::numOfWeapons--;
}

Weapon::Weapon(){
	Weapon::numOfWeapons++;
}

Weapon::Weapon(string s){
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

void Weapon::toString(){
	cout << this->name << " is " << this->height << " cms tall,  " << this->weight << " pounds in weight and " <<
		this->age << " years old" << endl;
}


bool Weapon::compareTo(Weapon& weapon){
	Weapon* ap = &weapon;
	return this->age == ap->getAge();
}

bool Weapon::compareTo(Weapon* ap){
	return this->age == ap->getAge();
}

Weapon& Weapon::operator++(){
	this->age++;
	return *this;
}

Weapon& Weapon::operator--(){
	this->age--;
	return *this;
}

Weapon& Weapon::operator++(int){
	this->age++;
	return *this;
}

Weapon& Weapon::operator--(int){
	this->age--;
	return *this;
}

bool Weapon::operator==(const Weapon& Weapon){
	return this->age == Weapon.age;
}

string Weapon::toJson(){
	return "{\"name\": \"" + this->name +  "\", " +
	"\"age\": \"" + to_string(this->age) +  "\", " +
	"\"weight\": \"" + to_string(this->weight) +  "\", " +
	"\"height\": \"" + to_string(this->height) +
	"\"}";
}

string Weapon::toStringArray(){
	return this->name + "," +
		to_string(this->age) + "," +
		to_string(this->weight) + "," +
		to_string(this->height);
}