#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#ifndef ANIMAL_H
#define ANIMAL_H

class Animal {
	private:
		int age;
		int height;
		int weight;
		string name;

		static int numOfAnimals;

	public:
		int getHeight(){ return this->height; }
		int getWeight(){ return this->weight; }
		int getAge(){ return this->age; }
		string getName(){ return this->name; }
		void setHeight(int cm){ height = cm; };
		void setWeight(int kg){ weight = kg; };
		void setAge(int year){ age = year; };
		void setName(string animalName){ name = animalName; };

		void getFamily() {
			cout << "We are animals" << endl;
		}

		virtual void getClass(){
			cout << "I'm an animal" << endl;
		}

		virtual void makeSound(){
			cout << "The Animal says grr" << endl;
		}

		// Constructors & Destructor
		Animal(int, int, int, string);
		Animal(string);
		~Animal();
		Animal();

		static int getNumOfAnimals(){ return numOfAnimals; };

		// Prototypes
		void toString();
		string toJson();
		string toStringArray();
		bool compareTo(Animal&);
		bool compareTo(Animal*);

		// Operators
		Animal& operator++();
		Animal& operator++(int);
    Animal& operator--();
    Animal& operator--(int);
		bool operator==(const Animal&);

};

#endif