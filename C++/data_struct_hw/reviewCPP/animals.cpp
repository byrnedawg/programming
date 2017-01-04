#include <iostream>

using namespace std;

class Animal {
 public:
    Animal() {} 
    double getWeight(double weight) const { return weight; }
 protected:
    double weight;
};

class Pet : public Animal {
  public:
    Pet() { }
};

int main()
{
  Pet myPet;
  Animal myAnimal;
  
  cout << myAnimal.getWeight(45) << endl;
  cout << myPet.getWeight(60) << endl;
  return 0;
}