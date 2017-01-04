//
// Created by hirsch on 22/04/16.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// #include "Animal.h"
#include "Dog.h"
#include "Cat.h"

using namespace std;

// void whatClassAreYou(Animal *ap){
// 	ap->getClass();
// }

int main() {

	// Animal a1(35, 13, 2, "tom");
	// Animal a2(23, 54, 3, "balou");
	// Animal a3(45, 23, 4, "max");
	// Animal a4(12, 34, 6, "timon");
	// Animal a5(55, 14, 1, "pumba");

	// Animal *animalsB[10];
  // animalsB[0] = new Animal(35, 13, 2, "tom");
  // animalsB[1] = new Dog(35, 13, 2, "tom", "wuff");

	Animal animals[10];
	animals[0] = Animal(35, 13, 2, "tom");
	animals[1] = Animal(23, 54, 3, "balou");
	animals[2] = Animal(45, 23, 4, "max");

	Animal timon(12, 34, 6, "timon");
	Animal pumba(55, 14, 6, "pumba");

	animals[3] = timon;
	animals[4] = pumba;


	Dog bello(35, 13, 2, "bello", "wuff");
	Dog fifi(23, 54, 3, "fifi", "wau");
	Cat garfield(45, 23, 4, "garfield", "lasagne");
	Cat ella(12, 34, 6, "ella", "meow");
	Cat mizzi(55, 14, 6, "mizzi", "mau");

	animals[5] = bello;
	animals[6] = fifi;
	animals[7] = garfield;
	animals[8] = ella;
	animals[9] = mizzi;

	Animal *ap = animals;
	cout << "We have this num of animals " << sizeof(animals) << endl;
	for (size_t i = 0; i < 10; i++){
		ap->toString();
		ap->getClass();
		ap++;
	}

	cout << "Does Timons and Pumba have the same age? " << (timon == pumba)  << endl;
	cout << "Does Garfield and Pumba have the same age? " << (garfield == pumba)  << endl;
	timon++;
	cout << "Does Timons and Pumba have the same age? " << (timon == pumba)  << endl;

	// Streams
	// 	Mode Flag	Description
	// ios::app	Append mode. All output to that file to be appended to the end.
	// ios::ate	Open a file for output and move the read/write control to the end of the file.
	// ios::in	Open a file for reading.
	// ios::out	Open a file for writing.
	// ios::trunc	If the file already exists, its contents will be truncated before opening the file.

	// File schreiben (fstream works too)
	ofstream writer("animals.txt");
	if(!writer){
		cout << "Error opening file" << endl;
		return -1;
	} else{
		// direct way
		//writer << garfield.toStringArray() << endl;
		// long way
		string row = garfield.toStringArray();
		const char *cstr = row.c_str();
		for (int i = 0; i < row.length(); i++){
			writer.put(*(cstr + i));
		}
		writer.close();
	}

	// File lesen (fstream works too)
	char letter;
	ifstream reader("animals.txt");
	if(!reader){
		cout << "Error opening file" << endl;
		return -1;
	}else{
		string line;
		for (int i = 0; !reader.eof(); i++){
			reader.get(letter);
			line += letter;
		}
		Animal fileLine(line);
		fileLine.toString();
		reader.close();
	}

	// FStream Line
	fstream f("animals.txt");
  string line;
  if (f.is_open()) {
		while (getline(f, line)){
			cout << line << endl;
		}
		f.close();
	} else {
		cout << "Unable to open file" << endl;
	}
	cout << "This is my f*** line " << line << endl;
	Animal fileLineB(line);
	fileLineB.toString();

	// everything went successful
	return 0;
}
