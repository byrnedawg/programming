#include <iostream>
#include <string>
#include <vector>

using namespace std;

class LinkedList2 {
private:
	class Node {
	public:
    int val;
    bool isEnd;
		Node* next;
		Node(int v, Node* n) : val(v), next(n), isEnd(0) {}
		Node(int v, bool end, Node* n) : val(v), next(n), isEnd(end) {}
	};
	Node* head;
	Node* tail;
	int count;
public:
	LinkedList2() : head(nullptr), tail(nullptr) 
	{
		count = 0;
	}
	
	~LinkedList2()
	{
		Node* q;
		for (Node* p = head; p != nullptr; p = q) 
		{
			q = p->next;
			delete p;
		}
	}
	
	void wordEnd(int v) 
	{
		if (head == nullptr) 
		{
			head = new Node(v, true, nullptr);
			tail = head;
		//	cout <<"\n The Head is pointing to " << head -> val << "\n";
		//	cout << "\n The tail is pointing to " << tail -> val << "\n";
		}
		else
		{
			tail -> next = new Node(v, true, nullptr);
			tail = tail->next;
		//	cout <<"\n The Head is pointing to " << head -> val << "\n";
		//	cout << "\n The tail is pointing to " << tail -> val << "\n";
		}
		
		count++;
	}

	void addEnd(int v) 
	{
		if (head == nullptr) 
		{
			head = new Node(v, nullptr);
			tail = head;
		//	cout <<"\n The Head is pointing to " << head -> val << "\n";
		//	cout << "\n The tail is pointing to " << tail -> val << "\n";
		}
		else
		{
			tail -> next = new Node(v, nullptr);
			tail = tail->next;
		//	cout <<"\n The Head is pointing to " << head -> val << "\n";
		//	cout << "\n The tail is pointing to " << tail -> val << "\n";
		}
		
		count++;
	}
	
	void addStart(int v) 
	{ 
		
		if(head == nullptr)
		{
			head = new Node(v, nullptr);
			tail = head;
		//	cout <<"\n The Head is pointing to " << head -> val << "\n";
		//	cout << "\n The tail is pointing to " << tail -> val << "\n";
		}
		else
		{
			Node *p = new Node(v, head);
			head = p;
		//	cout <<"\n The Head is pointing to " << head -> val << "\n";
		//	cout << "\n The tail is pointing to " << tail -> val << "\n";
		}
		count++;
		
	}
	
	void insert(int i, int v) 
	{
		
		if(i > size())
		{
			throw "LinkedList index out of bounds";
		}
		else if(i == size())
		{
			addEnd(v);
		}
		else if(i == 0)
		{
			addStart(v);
		}
		else
		{
			Node* p = head;
	    	while (i > 0) 
	    	{
				if (p == nullptr)
				{
					return;
				}
				p = p->next;
	    		i--;
			}
	    	p->next = new Node(v, p->next);
	    	count++;
		}
	}
	
	void removeEnd() 
	{
		if(head == nullptr)
		{
			cout << "Empty List \n";
			return;
		}
	
		if (head == tail)
		{
			Node* p = tail;
			head = nullptr;
			tail = nullptr;
			delete p;
		//	cout <<"\n The Head is pointing to nothing\n";
		//	cout << "\n The tail is pointing to nothing\n";
			count--;
		}
		else
		{
			Node* p;
			for (p = head; p->next->next != nullptr; p = p->next)
			{
				;
			}
				p->next = nullptr;
				tail = p;
				p = p -> next;
				delete p;
				count--;
		//		cout <<"\n The Head is pointing to " << head -> val << "\n";
		//		cout << "\n The tail is pointing to " << tail -> val << "\n";
		}
			
	}
	
	void removeStart() 
	{
		if(head == nullptr)
		{
			cout << "Empty List \n";
			return;
		}
		if (head == tail)
		{
			Node* p = tail;
			head = nullptr;
			tail = nullptr;
			delete p;
		//	cout <<"\n The Head is pointing to nothing \n";;
		//	cout << "\n The tail is pointing to nothing\n";
			count--;
		}
		else
		{
			Node *p = head;
			head = p-> next;
			p = nullptr;
			delete p;
		//	cout <<"\n The Head is pointing to " << head -> val << "\n";
		//	cout << "\n The tail is pointing to " << tail -> val << "\n";
			count --;
		}
	}

	friend ostream& operator<<(ostream& s, const LinkedList2& list) 
	{
	    s << "{ ";
		for (Node* p = list.head; p != nullptr; p = p->next) 
		{
			s << p->val << ' ';
		}
		s << "}";
		return s;
	}
 
	int size() const 
	{ 
		return count;
	}

	int get(int i) const 
	{ 
		if(i == size())
		{
			return tail->val;
		}
		else if(i > size())
		{
			return -1;
			throw "LinkedList index out of bounds";
		}
		else
		{
			Node* p = head;
			for ( ; i > 0; i--, p = p->next)
				if (p == nullptr)
					throw "LinkedList index out of bounds";
	    	
	    	
	    	
	    		cout <<"\n The Head is pointing to " << head -> val << "\n";
				if(p -> next != nullptr)
				cout << "\n The next value is pointing to " << p->next-> val << "\n";
				if(tail != nullptr)
				cout << "\n The Tail is pointing to " << tail -> val << "\n";
		
				return p->val;
		}
	}
	
	bool getEnd() const 
	{
		if(head == tail)
		{
			return tail->isEnd;
		}
		else if(head == nullptr)
		{
			return -1;
			throw "LinkedList index out of bounds";
		}
		else
		{
			Node* q;
			for (Node* p = head; p != nullptr; p = q) 
			{
				q = p->next;
				if(p->isEnd)
					return p->isEnd;
				
			}
	    	
	    	
	    	//	cout <<"\n The Head is pointing to " << head -> isEnd << "\n";
			//	if(p -> next != nullptr)
			//	cout << "\n The next value is pointing to " << p->next-> isEnd<< "\n";
			//	if(tail != nullptr)
			//	cout << "\n The Tail is pointing to " << tail -> isEnd << "\n";
		}
	}
};

class HashMap {
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
	HashMap(int size) : capacity(size), used(0) 
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
	
	friend ostream& operator<<(ostream& s, const HashMap& HashMap) 
	{
	    cout << "\t\t";
	    for(int i= 0; i < HashMap.capacity; i++)
	    {
        	cout << i << "\t";
	    }
	    s << "\n\t{ \t";
	    for(int i= 0; i < HashMap.capacity; i++)
	    {
        	cout << "[" << HashMap.table[i] << "]\t";
	    }
		s << "}";
		return s;
	}

};

void firstTest()
{
	HashMap myHash(10);
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

void secondTest()
{
	LinkedList2* arr[26];
	for(int i = 0; i < 26; i++)
	{
		arr[i] = new LinkedList2();
	}
	   
	LinkedList2* p;
	for(int j = 0; j < 26; j++)
	{
		p = arr[j];
		p->addEnd(char('a'+j));
		cout << "LinkedList =" << *p << "\n"; 
	} 
	p = arr[6];
	p->addEnd(char('g'));
	p->addEnd(char('r'));
    p->addEnd(char('e'));
    p->addEnd(char('g'));
    cout << "LinkedList =" << *p << "\n"; 
}

void thirdTest()
{
	int answer = 0;
	LinkedList2* arr[26];
	for(int i = 0; i < 26; i++)
	{
		arr[i] = new LinkedList2();
	}
	   
	LinkedList2* p;
	string inString;
	char inChar;
	do{
		cout << "Enter a string\n";
		cin >> inString;
		for(int i = 0; i < inString.length(); i++)
		{
			inChar = inString[i];
			//cout << inChar << ' ';
			if(i == 0)
			{
				p = arr[inChar - 'a'];
				p->addEnd(inChar);
			}
			else if(i == inString.length()-1)
			{
				p->wordEnd(inChar);
			}
			else
			{
		//	p = arr[2];
		//	p->addEnd(char('g'));
			p->addEnd(inChar);
			//cout << "LinkedList =" << *p << "\n"; 
			}
		}
		
		for(int i = 0; i < 26; i++)
		{
			p = arr[i];
			
			if(p->getEnd())
			{
				//cout << "The end is = " << p->getEnd() << "\n";
				cout << *p << "\n";
			}
		}
		
		
		cout << "Do another?\n";
		cin >> answer;
	}while(answer != 4);
//	const char* strdata = inString.c_str();

//	for (int i = 0; i < inString.length(); ++i)
    //	cout << i << strdata[i];

}

int main()
{
	   // firstTest();
	   //secondTest();
	   thirdTest();

   
    return 0;
}
