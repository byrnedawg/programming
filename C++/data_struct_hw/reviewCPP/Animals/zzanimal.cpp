//
// Created by hirsch on 22/04/16.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Animal {
	private:
		int height;
		int weight;
		string name;

		static int numOfAnimals;

	public:
		int getHeight(){ return this->height; }
		int getWeight(){ return this->weight; }
		string getName(){ return this->name; }
		void setHeight(int cm){ height = cm; };
		void setWeight(int kg){ weight = kg; };
		void setName(string animalName){ name = animalName; };

		// Prototype
		// void setAll(int, int, string);

		Animal(int, int, string);
		~Animal();
		Animal();

		static int getNumOfAnimals(){ return numOfAnimals; };
		void toString();
		bool compareTo(Animal&);
		bool compareTo(Animal*);

};

int Animal::numOfAnimals = 0;

Animal::Animal(int height, int weight, string name){
	this->height = height;
	this->weight = weight;
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

void Animal::toString(){
	cout << this->name << " is " << this->height << " cms tall and  " << this->weight << " kgs in weight" << endl;
}

bool Animal::compareTo(Animal& animal){
	Animal* ap = &animal;
	return this->weight == ap->getWeight();
}

bool Animal::compareTo(Animal* ap){
	return this->weight == ap->getWeight();
}

class Dog: public Animal{
	private:
		string sound = "Woof";
	public:
		void getSound(){ cout << this->sound << endl; };

		Dog(int, int, string, string);
		Dog() : Animal(){};
		void toString();
};

Dog::Dog(int height, int weight, string name, string bark) : Animal(height, weight, name){
	this->sound = bark;
}

void Dog::toString(){
	cout << this->getName() << " is " << this->getHeight() << " cms tall,  " << this->getWeight() << " kgs in weight and says " << this->sound << endl;
}

int main() {

	Animal fred;

	fred.setHeight(33);
	fred.setName("fred");
	fred.toString();

	Animal tom(36, 15, "tom");
	tom.toString();

	Dog balou(44, 23, "balou", "wufff");
	balou.toString();
	balou.Animal::toString();

	Animal* fp = &fred;
	cout << "Compare To: " << balou.compareTo(fred) << endl;
	cout << "Compare To: " << balou.compareTo(fp) << endl;
	balou.setWeight(10);
	fred.setWeight(10);
	cout << "Compare To: " << balou.compareTo(fred) << endl;

	cout << Animal::getNumOfAnimals() << endl;
    // everything went successful
    return 0;
}
