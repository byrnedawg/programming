###Generel
####Variables
```c++
// Arrays
int badNums[5] = {4, 13, 14, 24, 24};
char myName[4][6] = {{'G','e','r','y'}, {'H','i','r','s','c','h'}}

// Typen umwandlung
string yourName;
getline(cin, yourName);
cout << yourName << endl;
int a = stoi("4");
double b = stod("4.5");

// has a function-pointer as parameter

void quickSort(T* a, int start, int end, int (*cmp)(T, T)){...}
```

####Function as parameter or as Lambda
```c++
// declare type for fraction comparing function
typedef int (*fractionComparator)(Fraction, Fraction);  
// prototype for our comparator function 
int compareFractions(Fraction a, Fraction b);           
void quickSort(T* a, int start, int end, fractionComparator cmp){...} 
```

```c++
class FractionComparator{  // this is the functor for compare Fractions
  public:
    int operator()(Fraction a, Fraction b){...}
};

template<typename T> void quickSort(T*, int, int, function<int(T, T)>);
```

```c++
function<int(Fraction, Fraction)> comparator = [] (Fraction a, Fraction b) {...};  // make a function-object by a lambda expression
```

####Headers
```c++
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Animal.h"

using namespace std;
```

####Pointers & References
```c++
void makeMeYoung(int* age){ *age = 18; }
void actYourAge(int& age){ age = 28; }

int main() {
	int myAge = 28;
	int* agePtr = &myAge;
	cout << "Addreess of the pointer: " << agePtr << endl;
	cout << "Data at memory: " << *agePtr << endl;
	int* badNumPtr = badNums;
	cout << "Addreess : " << badNumPtr << endl;
	cout << "Value : " << *badNumPtr << endl;
	badNumPtr++;
	cout << "Addreess : " << badNumPtr << endl;
	cout << "Value : " << *badNumPtr << endl;
	makeMeYoung(&myAge);
	cout << "Now I am " << *agePtr << endl;
	int& myAgeRef = myAge;
	cout << "my age is " << myAge << endl;
	myAgeRef++;
	cout << "my age is " << myAge << endl;
	actYourAge(myAgeRef);
	cout << "my age is " << myAge << endl;
}
```

```c++
const char *cstr = row.c_str();
for (int i = 0; i < row.length(); i++){ writer.put(*(cstr + i)); }
```

__Output__

```c++
Addreess of the pointer: 0x7fff5f2028dc
Data at memory: 28
Addreess : 0x7fff5f202900
Value : 4
Addreess : 0x7fff5f202904
Value : 13
I use to be 28
Now I am 18
my age is 18
my age is 19
my age is 28
```

###Templates
```c++
// Old code without the templates
// void output(int i){ cout << i << endl; }
// void output(float f){ cout << f << endl; }
// void output(char c){ cout << c << endl; }
// void output(const char* word){ cout << word << endl; }

// instead of class you can use typename
template <class T>
void output(const T & n){ cout << n << endl; }

template <class T>
void swapThis(T & a, T & b){
 T tmp = a;
 a = b;
 b = tmp;
}

template <class T>
class A {
	private:
		T data;
	public:
		A(const T& init);
		void output();
};

template <class T>
A<T>::A(const T& init){ data = init; }

template <class T>
void A<T>::output(){ cout << data << endl; }

int main() {
	// Functions
	output(1);
	output(2.34);
	output('a');
	output("wurst");

	int array[5] = { 1,3,5,7,9 };
	swapThis(array[1], array[3]);
	for(int i=0; i < 5; i++){ cout << array[i] << endl; }

	// Classes
	A<int> number(5);
	A<char> letter('c');
	number.output();
	letter.output();

	// everything went successful
	return 0;
}
```

###FileStreams
#### Generel
```c++
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
	for (int i = 0; i < row.length(); i++){ writer.put(*(cstr + i)); }
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
	while (getline(f, line)){ cout << line << endl; }
	f.close();
} else {
	cout << "Unable to open file" << endl;
}
cout << "This is my f*** line " << line << endl;
Animal fileLineB(line);
fileLineB.toString();
```

####Random File Access
```c++
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

int main(){
	FILE* fPointer;
	// w+ means write it first and than read it
	fPointer = fopen("RandomFileAccess.txt", "w+");
	fputs("I ate 3 pumpkins today", fPointer);
	// SEEK_SET = From the begining of the file
	// SEEK_CUR = From the current position of the file
	// SEEK_END = From the end of the file
	fseek(fPointer, 7, SEEK_SET);
	fputs(" munchikins on Friday", fPointer);
	// -> I ate 3 munchikins on Friday
	fseek(fPointer, -6, SEEK_END);
	fputs("top of a mountain", fPointer);
	// -> I ate 3 munchikins on top of a mountain

	// BINARY
	double value;
	char* pmemory = (char*) &value;

	// Open binary file to write:
	// ios::app   -- Append to the file
	// ios::ate   -- Set the current position to the end
	// ios::trunc -- Delete everything in the file
	ofstream out_file("RandomFileAccess.bin", ios::out | ios::binary | ios::trunc);
	while (cin >> value) {
		out_file.write(pmemory, sizeof(double));
	}
	out_file.close();

	// Open binary file to read:
	ifstream in_file("RandomFileAccess.bin", ios::in | ios::binary);
	in_file.read(pmemory, sizeof(double));

	while (!in_file.eof()) {
			cout << value << endl;
			in_file.read(pmemory, sizeof(double));
	}
	in_file.close();

	return 0;
}
```

###STL
####Iterators, list and vectors
```c++
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <algorithm>

int main(){
	vector<int> vec;
	vec.push_back(21);
	vec.push_back(7);
	vec.push_back(14);
	// Iterators
	vector<int>::iterator itrBeg = vec.begin();
	vector<int>::iterator itrEnd = vec.end();

	for (vector<int>::iterator itrLoop = itrBeg; itrLoop != itrEnd; itrLoop++){
		cout << *itrLoop << endl;
	}
	//21 7 14

	list<int> integer_list;
	int_list.push_front(1);
	int_list.push_front(2);
	int_list.push_front(3);
	for(list<int>::iterator list_iter = int_list.begin(); list_iter != int_list.end(); list_iter++){
    cout << *list_iter << endl;
	}
	// 3 2 1

	return 0;
}
```

###Lamdas
```c++
int limit = 1200;cout << count_if(begin(v), end(v), [limit](int i) {return i < limit;});
```



###Classes
__Animal.h__

```c++
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

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

		void getFamily() { cout << "We are animals" << endl; }
		virtual void getClass(){ cout << "I'm an animal" << endl; }
		virtual void makeSound(){ cout << "The Animal says grr" << endl; }

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
```

__Animal.cpp__

```c++
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

Animal::Animal(){ Animal::numOfAnimals++; }

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
```

__Dog.h__

```c++
#include "Animal.h"

class Dog: public Animal {
	private:
		string sound;

	public:
		void getClass(){ cout << "I'm a Dog" << endl; }
		void makeSound(){ cout << "The Dog says " << this->sound << endl; }

		Dog() : Animal() {};
		Dog(int, int, int, string, string);

		void toString();
};
```

__Dog.cpp__

```c++
#include "Dog.h"

Dog::Dog(int height, int weight, int age, string name, string sound) :
	Animal(height, weight, age, name){
		this->sound = sound;
}

void Dog::toString(){
	cout << this->getName() << " is " << this->getHeight() << " cms tall,  " <<
		this->getWeight() << " kgs in weight, " << this->getAge() << " years old and says " <<
		this->sound << endl;
}
```

__main.cpp__

```c++
Animal a1(35, 13, 2, "tom");
Animal a2(23, 54, 3, "balou");
Dog bello(35, 13, 2, "bello", "wuff");

Animal animals[10];
animals[0] = Animal(35, 13, 2, "tom");
animals[1] = Animal(23, 54, 3, "balou");
Animal *ap = animals;
for (size_t i = 0; i < 10; i++){
	ap->toString();
	ap++;
}

```


