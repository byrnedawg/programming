#include <iostream>
using namespace std;

class GrowArray {
private:
	int* p;
	int size;
	int used;
	// this should double the size of your list
	void grow() 
	{
		size = size * 2;
	}
public:
	GrowArray() : p(new int[1]), size(1), used(0) {}
	GrowArray(int capacity) : p(new int[capacity]), size(capacity), used(0) {}
	~GrowArray() 
	{
		delete [] p;
	}
	
	void addEnd(int v)  //O(    )
 	{  
		if (used == size)
			grow();
			
		p[used] = v;
		used++;

	}
	
	void addStart(int v) // O(   )
	{ 
		if (used == size)
			grow();
			
		int* temp = p;
		p = new int[used+1];
		for (int i = 0; i < used; i++) //O(n)
			p[i+1] = temp[i];
		p[0] = v;
		used++;
	}
	
	void removeEnd() { //O(1)
		used--;
	}
	
	void removeStart() { //O(n)
		for (int i = 0; i < used-1; i++)
			p[i] = p[i+1];
		used--;
	}
	
	int getCapacity() const 
	{
    	return size;		
	}

	int getUsed() const 
	{
		return used;
	}
	
	int get(int i) const //O(1)
	{
		if (i < 0 || i >= used)
			throw "Out of Bounds";
		return p[i];
	}
	
	void set(int i, int v) // O(1)
	{ 
		if (i < 0 || i >= used)
				throw "Out of Bounds";
		p[i] = v;
	}
	
	int operator[](int i) const 
	{
		if (i < 0 || i >= used)
			throw "Out of Bounds";
		return p[i];
	}
	
	int& operator[](int i) 
	{
		if (i < 0 || i >= used)
			throw "Out of Bounds";
		return p[i];
	}
	
	friend ostream& operator<<(ostream& s, const GrowArray& gArray) 
	{
		s << "{ ";
		for (int i = 0; i < gArray.used; i++) {
			s << gArray[i] << ' ';
		}
		s << "}";
		return s;
	}
	
};

class BadGrowArray {
private:
	int* data;
	int size;
public:
	BadGrowArray() {
		data = nullptr; // O(1)
		size = 0;
	}
	BadGrowArray(int initialSize) {
		data = new int[initialSize]; //O(1)
		size = initialSize;
	}
	~BadGrowArray() {
		delete [] data;
	}
	void addEnd(int v) {   //O(n)
		int* temp = data; // O(1)
		
    data = new int[size+1]; // O(1)
		for (int i = 0; i < size; i++) //O(n)
			data[i] = temp[i];
		data[size] = v;
		size++;
	}
	void addStart(int v) { //O(n)
		int* temp = data;
		data = new int[size+1];
		for (int i = 0; i < size; i++) //O(n)
			data[i+1] = temp[i];
		data[0] = v;
		size++;
	}
	void removeEnd() { //O(1)
		size--;
	}
	void removeStart() { //O(n)
		for (int i = 0; i < size-1; i++)
			data[i] = data[i+1];
		size--;
	}
	
	int operator[](int i) const 
	{
		return data[i];
	}
	
	int& operator[](int i) 
	{
		return data[i];
	}
	
	friend ostream& operator<<(ostream& s, const BadGrowArray& badArray) 
	{
		s << "{ ";
		for (int i = 0; i < badArray.size; i++) {
			s << badArray[i] << ' ';
		}
		s << "}";
		return s;
	}
};

int main() 
{
	BadGrowArray a;
	GrowArray b;
	GrowArray c(23);
	for (int i = 0; i < 50; i++)
	{
		a.addEnd(i);
		a.addStart(i+2);
		b.addEnd(i);
	}
	a[5] = 201;
	cout << a << "\n\n\n";
	cout << b << "\n";
	cout << "Storage used = " << b.getUsed() << "\tCapacity = " << b.getCapacity() << "\n";
	
	for(int j = 0; j < 7; j++)
	{
		b.addStart(13);
		b.addStart(53);
	}
	b.addStart(74);
	cout << b << "\n";
	cout << "Storage used = " << b.getUsed() << "\tCapacity = " << b.getCapacity() << "\n";
	b.removeStart();
	b.removeEnd();
	cout << b << "\n";
	cout << "Storage used = " << b.getUsed() << "\tCapacity = " << b.getCapacity() << "\n";
	c.addStart(13);
	c.addEnd(53);
	cout << c << "\n";
	cout << "Storage used = " << c.getUsed() << "\tCapacity = " << c.getCapacity() << "\n";
	
    unsigned long long testNumber;
    ifstream myfile ("HW4a.txt");
    if (myfile.is_open())
    {
        while  (myfile >> testNumber)
        {
            cout <<  testNumber << "\t";
        }
        myfile.close();
    }

  else cout << "Unable to open file"; 
}


