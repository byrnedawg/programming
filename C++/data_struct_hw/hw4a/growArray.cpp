// Name: Gregory Byrne
// Assignment: HW4a - growArray
#include <iostream>
#include <fstream>
#include <string>


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
		
		int* temp = p; // O(1)
    	p = new int[size]; // O(1)
		for (int i = 0; i <= used; i++)
		{
			p[i] = temp[i];
		}
	}
public:
	GrowArray() : p(new int[1]), size(1), used(0) {}
	GrowArray(int capacity) : p(new int[capacity]), size(capacity), used(0) {}
	~GrowArray() 
	{
		delete [] p;
		//cout << "\n\nKilled the object\n\n\n";
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
		for (int i = used; i >= 0; i--) //O(n)
			p[i] = p[i-1];
		p[0] = v;
		used++;
	}
	
	int removeEnd() //O(1)
	{ 
		int temp; 
		if (used > 0)
		{
			temp=p[used-1];
			used--;
			return temp;
		}
		else
		{
			cout << "Cant Delete Empty Array\n";
			return -1;
		}
		
	}
	
	int removeStart()    //O(n)
	{ 
		int temp; 
		if (used > 0)
		{
			temp = p[0];
			for (int i = 0; i < used-1; i++)
				p[i] = p[i+1];
			used--;
			return temp;
		}
		else
		{
			cout << "Cant Delete Empty Array\n";
			return -1;
		}
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

void firstTest()
{
	GrowArray b;
	GrowArray c(23);
	for (int i = 0; i < 32; i++)
	{
		//b.addEnd(i);
		b.addStart(i);
	}
	cout << b << "\n";
	cout << "Storage used = " << b.getUsed() << "\tCapacity = " << b.getCapacity() << "\n";
	
	for(int j = 1; j < 33; j++)
	{
	//	b.addStart(j);
		b.addEnd(j);
	}
	b.addStart(74);
	cout << b << "\n";
	cout << "Storage used = " << b.getUsed() << "\tCapacity = " << b.getCapacity() << "\n";
	b.removeStart();
	b.removeStart();
	b.removeEnd();
	b.removeEnd();
	cout << b << "\n";
	cout << "Storage used = " << b.getUsed() << "\tCapacity = " << b.getCapacity() << "\n";
	c.addStart(13);
	c.addEnd(53);
	cout << c << "\n";
	cout << "Storage used = " << c.getUsed() << "\tCapacity = " << c.getCapacity() << "\n";
}

void openTestFile()
{
	GrowArray testArray;
	ifstream myfile ("HW4a.txt");
//	ifstream myfile ("HW4b.txt");
	string::size_type sz;
    string command;
    string line;
    string numbers = "0123456789";
    string stringnum1, stringnum2, stringnum3, stringRemove;
    int startNum, stepNum, stopNum;
    string addFront = "ADD_FRONT";
    string addBack = "ADD_BACK";
    string output1 = "OUTPUT";
    string output2 = "OUTPUT\r";
    string removeFront = "REMOVE_FRONT";
    string removeBack = "REMOVE_BACK";
    
    if (myfile.is_open())
	{
    	while  (getline(myfile, line))
    	{
    		command = line.substr(0, line.find_first_of(' '));
    		if(command.compare("") != 0 && command.compare("\r")!= 0 )
    		{
	    		if((command.compare(addFront) == 0))
	    		{
	    			stringnum1 = line.substr(line.find_first_of(numbers.c_str()), line.find_first_of(':') - line.find_first_of(numbers.c_str()));
	    			stringnum2 = line.substr(line.find_first_of(':')+1,(line.find_last_of(':'))-line.find_first_of(':')-1);
	   				stringnum3 = line.substr(line.find_last_of(':')+1);
	   				startNum = stoi(stringnum1, &sz);
	   				stepNum = stoi(stringnum2, &sz);
	   				stopNum = stoi(stringnum3, &sz);
	    		//	cout << "The command for this line is ADD_FRONT\n";
		    	//	cout << "StartNum = " << startNum << "\n";
		    	//	cout << "StepNum = " << stepNum << "\n";
		   		//	cout << "StopNum = " << stopNum << "\n";
		   			for(int i = startNum; i <= stopNum; i= i+stepNum)
		   			{
		   				testArray.addStart(i);
		   			}
	   			}
	   			else if((command.compare(addBack) == 0))
	   			{
	   				stringnum1 = line.substr(line.find_first_of(numbers.c_str()), line.find_first_of(':') - line.find_first_of(numbers.c_str()));
	    			stringnum2 = line.substr(line.find_first_of(':')+1,(line.find_last_of(':'))-line.find_first_of(':')-1);
	   				stringnum3 = line.substr(line.find_last_of(':')+1);
	   				startNum = stoi(stringnum1, &sz);
	   				stepNum = stoi(stringnum2, &sz);
	   				stopNum = stoi(stringnum3, &sz);
	   			//	cout << "The command for this line is ADD_BACK\n";
	   			//	cout << "StartNum = " << startNum << "\n";
		    	//	cout << "StepNum = " << stepNum << "\n";
		   		//	cout << "StopNum = " << stopNum << "\n";
		   			for(int i = startNum; i <= stopNum; i = i+stepNum)
		   			{
		   				testArray.addEnd(i);
		   			}
	    		}
	    		else if((command.compare(output1) == 0) || (command.compare(output2) == 0))
	    		{
	    			//cout << "The command for this line is OUTPUT\n";
	    			cout << testArray << "\n";
					cout << "Storage Used = " << testArray.getUsed() << endl;
					cout << "Total Capacity = " << testArray.getCapacity() << "\n";
	    		}
	    		else if((command.compare(removeFront) == 0))
	    		{
					stringRemove = line.substr(line.find_first_of(numbers.c_str()));
					startNum = stoi(stringRemove, &sz);
	    		//	cout << "The command for this line is REMOVE_FRONT\n";	
	    		//	cout << "RemoveNum= " << startNum << "\n";
	    			
	    			for(int i = 0; i < startNum; i++)
	    			{
	    				testArray.removeStart();
	    			}
	    		}
	    		else if((command.compare(removeBack) == 0))
	    		{
	    			stringRemove = line.substr(line.find_first_of(numbers.c_str()));
					startNum = stoi(stringRemove, &sz);
	    		//	cout << "The command for this line is REMOVE_BACK\n";	
	    		//	cout << "RemoveNum= " << startNum << "\n";
	    			for(int i = 0; i < startNum; i++)
	    			{
	    				testArray.removeEnd();
	    			}
	    		}
	    		else
	    		{
	    			cout << "The Text File may be corrupt no understood commands\n\n";
	    		}	
		    	
    		}
	    		
    	
    	}
    	myfile.close();
	}
	else
    {
        cout << "Unable to open file";
    }
}

void commandLineTest()
{
	GrowArray commandLineArray;
	int answer = 0;
	int value = 0;
	cout << "Here is the current Array: \n";
	cout << commandLineArray << "\n";
	cout << "Storage Used = " << commandLineArray.getUsed() << "\n";				
	cout << "Total Capacity = " << commandLineArray.getCapacity() << "\n";
	do{
		cout << "-----------------------------------------------------------\n";
		cout << "What do you want to do to our Array? \n";
		cout << "1: Add value to the End \n";
		cout << "2: Add value to the Front \n";
		cout << "3: Remove value from the End \n";
		cout << "4: Remove value from the Front \n";
		cout << "5: Dispplay Array and Stats \n";
		cout << "6: Finished End Program \n";
		cout << "-----------------------------------------------------------\n";
		cin >> answer;
		if(answer == 1)
		{
			cout << "What Value do you want to add to the End? \n";
			cin >> value;
			commandLineArray.addEnd(value);
			cout << "Adding " << value << " to the end \n\n";
			cout << commandLineArray << "\n";
			cout << "Storage Used = " << commandLineArray.getUsed() << "\n";				
			cout << "Total Capacity = " << commandLineArray.getCapacity() << "\n";
			
		}
		else if(answer == 2)
		{
			cout << "What Value do you want to add to the Front? \n";
			cin >> value;
			commandLineArray.addStart(value);
			cout << "Adding " << value << " to the Front \n\n";
			cout << commandLineArray << "\n";
			cout << "Storage Used = " << commandLineArray.getUsed() << "\n";				
			cout << "Total Capacity = " << commandLineArray.getCapacity() << "\n";
		}
		else if(answer == 3)
		{
			cout << "Removing " << commandLineArray.removeEnd() << " from the End \n\n";
			cout << commandLineArray << "\n";
			cout << "Storage Used = " << commandLineArray.getUsed() << "\n";				
			cout << "Total Capacity = " << commandLineArray.getCapacity() << "\n";
		}
		else if(answer == 4)
		{
			cout << "Removing " << commandLineArray.removeStart() << " from the Front \n\n";
			cout << commandLineArray << "\n";
			cout << "Storage Used = " << commandLineArray.getUsed() << "\n";				
			cout << "Total Capacity = " << commandLineArray.getCapacity() << "\n";
		}
		else
		{
			cout << "-----------------------------------------------------------\n";
			cout << commandLineArray << "\n";
			cout << "Storage Used = " << commandLineArray.getUsed() << "\n";				
			cout << "Total Capacity = " << commandLineArray.getCapacity() << "\n";
			cout << "-----------------------------------------------------------\n";
		}
	}while(answer != 6);
	
}


int main() 
{
	openTestFile();
//	commandLineTest();
//	firstTest();

  return 0;
}


