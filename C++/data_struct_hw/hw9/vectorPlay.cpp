#include <iostream>
#include <vector>
//#include <algorithm>
using namespace std;

vector<int>myVector;

vector<string>mystringVec;


int main() 
{
	myVector.push_back(1);
	myVector.push_back(2);
	myVector.push_back(3);
	myVector.push_back(4);
	myVector.push_back(5);
	
	mystringVec.push_back("hello");
	
	int goon = 	myVector.back();
	
	for(int i = 0; i < myVector.size(); i++)
	{
		cout << myVector[i];
	}
	
	for(int i = 0; i < mystringVec.size(); i++)
	{
		cout << mystringVec[i];
	}
	
	//cout << goon;
	return 0;
}
