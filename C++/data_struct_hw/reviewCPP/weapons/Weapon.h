#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#ifndef WEAPON_H
#define WEAPON_H

class Weapon {
	private:
		int age;
		int height;
		int weight;
		string name;

		static int numOfWeapons;

	public:
		int getHeight(){ return this->height; }
		int getWeight(){ return this->weight; }
		int getAge(){ return this->age; }
		string getName(){ return this->name; }
		void setHeight(int cm){ height = cm; };
		void setWeight(int kg){ weight = kg; };
		void setAge(int year){ age = year; };
		void setName(string WeaponName){ name = WeaponName; };

		void getFamily() {
			cout << "We are Weapons" << endl;
		}

		virtual void getClass(){
			cout << "I'm an Weapon" << endl;
		}

		virtual void makeSound(){
			cout << "The Weapon says bang" << endl;
		}

		// Constructors & Destructor
		Weapon(int, int, int, string);
		Weapon(string);
		~Weapon();
		Weapon();

		static int getNumOfWeapons(){ return numOfWeapons; };

		// Prototypes
		void toString();
		string toJson();
		string toStringArray();
		
		bool compareTo(Weapon&);
		bool compareTo(Weapon*);
        
		// Operators
		Weapon& operator++();
		Weapon& operator++(int);
    Weapon& operator--();
    Weapon& operator--(int);
		bool operator==(const Weapon&);

};

#endif