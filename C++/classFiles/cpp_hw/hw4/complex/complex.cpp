// Name: Gregory Byrne
// Assignment: HW4a - Complex
#include <iostream>
#include <cmath>


using namespace std;

class Complex {
private:
	double real, image; //private values
public:
	Complex(double real = 0.0, double image = 0.0) : real(real), image(image) {
		// 2 doubles for real and image parts of a complex number
	}
	
    Complex add(Complex b) const {
		// add reals and add image
		return Complex(this->real + b.real, this->image + b.image);
	}
	
	 Complex sub(Complex b) const {
		//sub reals and sub image
		return Complex(this->real - b.real, this->image - b.image);
	}
	Complex mult(Complex b) const {
	    return Complex(this->real * b.real, this->image * b.image);
	}
	
	void print() const {
    	if(image >= 0) // check to see if the imaginary part is positive
    	{
    		cout << real << "+" << abs(image) <<"i\n";
    	}
    	else
    	{
    	    cout << real << image <<"i\n";
    	}
	}
	
};

int main() {
	const Complex c1; // (real=0, imag =0)
	const Complex c2(1.5);// real=1.5, imag = 0.0
	Complex c3(1.5,2.25); // real = 1.5, imag=2.25

	cout << "c1 = ";
    c1.print(); // 0+0i
    cout << "c2 = ";
    c2.print(); // 1.5
    cout << "c3 = ";
	c3.print(); // 1.5+2.25i
	Complex c4 = c1.add(c2); // (real=1.5, imag=0.0)
    Complex c5 = c2.sub(c4); // 
    Complex c6 = c2.mult(c3);
    cout << "add c1 to c2 is c4 = ";
	c4.print();
	cout << "sub c4 from c2 is c5 = ";
	c5.print();
	cout << "mult c2 and c3 is c6 = ";
	c6.print();
}
