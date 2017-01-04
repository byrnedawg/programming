//
// Created by hirsch on 22/04/16.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

void makeMeYoung(int* age){
 cout << "I use to be " << *age << endl;
 *age = 18;
}

void actYourAge(int& age){
    age = 28;
}

int main() {
    // Arrays
    int badNums[5] = {4, 13, 14, 24, 24};
    char myName[4][6] = {{'G','e','r','y'}, {'H','i','r','s','c','h'}};

    // Typen umwandlung
    // string yourName;
    // getline(cin, yourName);
    // cout << yourName << endl;
    // int a = stoi("4");
    // double b = stod("4.5");

    // // File schreiben
    // string quote = "Bubu says hi!";
    // ofstream writer("bubu.txt");
    // if(!writer){
    //     cout << "Error opening file" << endl;
    //     return -1;
    // } else{
    //     writer << quote << endl;
    //     writer.close();
    // }

    // // File lesen
    // char letter;
    // ifstream reader("bubu.txt");
    // if(!reader){
    //     cout << "Error opening file" << endl;
    //     return -1;
    // }else{
    //     for (int i = 0; !reader.eof(); i++){
    //         reader.get(letter);
    //         cout << letter;
    //     }
    //     cout << endl;
    //     reader.close();
    // }


    // Pointers & References
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



    // everything went successful
    return 0;
}
