//
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Weapon.h"
#include "Mortar.h"
//#include "Cat.h"

using namespace std;

 void whatClassAreYou(Weapon *ap){
 	ap->getClass();
 }

int main() 
{
    
    const int arraySize = 4;
    Weapon guns[arraySize];
	guns[0] = Weapon(100, 25, 1, "60 mm");
	guns[1] = Weapon(150, 65, 1, "81 mm");
	guns[2] = Weapon(200, 95, 1, "120 mm");
	guns[3] = Mortar(100, 25, 2, "60mm", "Target Aquired");
	
	
	//cout << "gun in array pos 0 age = " << "\n";
	
	Weapon *ap = guns;
	cout << "We have this num of weapons " << sizeof(guns)/sizeof(Weapon) << endl;
	for (size_t i = 0; i < arraySize; i++){
		ap->toString();
		ap->getClass();
		ap->makeSound();
		ap++;
	}


	return 0;
}
