#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Weapon.h"

using namespace std;

class Mortar: public Weapon {
	private:
		string sound;

	public:
		void getClass(){
			cout << "I'm a Mortar" << endl;
		}
		void makeSound(){
			cout << "Mortar go boom " << this->sound << endl;
		}

		Mortar() : Weapon() {};
		Mortar(int, int, int, string, string);

		void toString();
};