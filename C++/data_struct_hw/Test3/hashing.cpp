#include <iostream>

using namespace std;

class HashSet {
private:
	int* table;
	int capacity;
	int used;
	
	void grow() 
	{
		int* temp = table;
		int oldCapacity = capacity;
		capacity = capacity*2;
		table = new int[capacity];
		used = 0;
		for (int i = 0; i < oldCapacity; i++)
		{
			if (temp[i] != 0)
			{
			    add(temp[i]);
			}
				
		}
		
	}
public:
	HashSet(int size) : capacity(size), used(0) 
	{
		table = new int[capacity];
		for (int i = 0; i < capacity; i++)
			table[i] = 0;
	}
	
	int hash(int v) 
	{
        return v % capacity;
	}

	void add(int v) 
	{
		if (used + used >= capacity)
			grow();
		int i = hash(v);
		while (table[i] != 0) 
		{
			i++;
			if (i == capacity)
			{
				i = 0;
			}
		}
		table[i] = v;
		used++;
	}
	
	bool contains(int v) 
	{
        int hashV = hash(v);
        int numChecks = 1;
        while(table[hashV] != 0)
        {
            if(table[hashV] == v)
            {
                    cout << "Number of hash look ups = " << numChecks << "\n";
                    return true;
            }
            else
            {
                    if(hashV < capacity-1)
                    {
                        hashV++;
                    }
                    else
                    {
                        hashV = 0;
                    }
                    numChecks++;
            }
        }
        cout << "Number of hash look ups = " << numChecks << "\n";
        return false;
	}
	
	int getCapacity() 
	{
        return capacity;
	}
	
	int getUsed() 
	{
        return used;
	}
	
	friend ostream& operator<<(ostream& s, const HashSet& hashSet) 
	{
	    cout << "\t\t";
	    for(int i= 0; i < hashSet.capacity; i++)
	    {
        	cout << i << "\t";
	    }
	    s << "\n\t{ \t";
	    for(int i= 0; i < hashSet.capacity; i++)
	    {
        	cout << "[" << hashSet.table[i] << "]\t";
	    }
		s << "}";
		return s;
	}

};

void firstTest()
{
	HashSet myHash(10);
	int answer = 0;
	int inValue = 0;

	do{
		cout << "-----------------------------------------------------------\n";
		cout << "Hash Set Options \n";
		cout << "1: Add a Value to Set \n";
		cout << "2: Determine if value is in the set \n";
		cout << "3: Print the Hash set \n";
		cout << "4: Finished End Program \n";
		cout << "-----------------------------------------------------------\n";
		cin >> answer;
		if(answer == 1)
		{
			cout << "What value do you want to add to the set?\n";
			cin >> inValue;
			myHash.add(inValue);
			cout << "-----------------------------------------------------------\n";
			cout << myHash << "\n";
			cout << "-----------------------------------------------------------\n";
		}
		else if(answer == 2)
		{
		    cout << "What value do you want to see is in the set?\n";
			cin >> inValue;
			if(myHash.contains(inValue))
			{
			    cout << inValue << " was IN the hash set\n";
			}
			else
			{
			    cout << inValue << " was NOT in the hash set\n";
			}
			cout << "-----------------------------------------------------------\n";
			cout << myHash << "\n";
			cout << "-----------------------------------------------------------\n";
		}
		else if(answer == 3)
		{
		    cout << "This is what the hash set looks like\n";
			cout << "-----------------------------------------------------------\n";
			cout << "Hash Set Used = " << myHash.getUsed() << "\n";
			cout << "Hash Set Capacity = " << myHash.getCapacity() << "\n";
			cout << myHash << "\n";
			cout << "-----------------------------------------------------------\n";
		}
		else
		{
		   cout << "This is what the hash set looks like\n";
			cout << "-----------------------------------------------------------\n";
			cout << "Hash Set Used = " << myHash.getUsed() << "\n";
			cout << "Hash Set Capacity  = " << myHash.getCapacity() << "\n";
			cout << myHash << "\n";
			cout << "-----------------------------------------------------------\n";
		}
	}while(answer != 4);

}

int main()
{
    firstTest();
    return 0;
}
