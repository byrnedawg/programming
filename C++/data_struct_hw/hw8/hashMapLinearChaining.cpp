// Name: Gregory Byrne
// Assignment: HW8 - Hash Map Linear Chaining
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int HISTSIZE = 11;
const int HASHSIZE = 500000;

class HashMap {
	private:
		class LinkedList2 {
		private:
			class Node {
			public:
		    string val;
				Node* next;
				Node(string v, Node* n) : val(v), next(n){}
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
			
			void empty()
			{
				if (head == nullptr) 
				{
					head = new Node("", nullptr);
					tail = head;
				//	cout <<"\n The Head is pointing to " << head -> val << "\n";
				//	cout << "\n The tail is pointing to " << tail -> val << "\n";
				}
				else
				{
					tail -> next = new Node("", nullptr);
					tail = tail->next;
				//	cout <<"\n The Head is pointing to " << head -> val << "\n";
				//	cout << "\n The tail is pointing to " << tail -> val << "\n";
				}
				
				count++;
			}
		
			void addEnd(string v) 
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
		
			void addStart(string v) 
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
			
		
			string get(int i)  const
			{ 
				if(i == size())
				{
					return tail->val;
				}
				else if(i > size())
				{
					return "null";
					throw "LinkedList index out of bounds";
				}
				else
				{
					Node* p = head;
					for ( ; i > 0; i--, p = p->next)
						if (p == nullptr)
						{
							cout << "LinkedList index out of bounds \n";
							return "null";
						}
				
						return p->val;
				}
			}
		
		};
		int capacity;
		int used;
		LinkedList2** table;
		int hist[HISTSIZE];
		
	public:
		HashMap(int cap):capacity(cap), used(0)
		{
			
			table = new LinkedList2*[capacity];
			for(int i = 0; i < capacity; i++)
			{
				table[i] = nullptr;
			//	table[i] = new LinkedList2();
			}
			
			for (int i = 0; i < HISTSIZE; i++)
			{
				hist[i] = 0;
			}
		}
		~HashMap() 
		{
			for (int i = 0; i < capacity; i++)
				delete table[i];
			delete [] table;
		}
	
		int hash(const string& s) const 
		{
			int sum = 0;
			for (int i = 0; i < s.size(); i++)
				sum = sum * 2 + s[i];
			return sum%capacity;
		}
		
	
	//	int hash(const string& s) const 
	//	{
	//		int sum = 0;
	//		for (int i = 0; i < s.size(); i++)
	//			sum = sum + s[i];
	//		return sum%capacity;
//		}
		
		void add(string s) 
		{
			LinkedList2* p;
		//	int i = s[0] -'a'; // make word go into bin of first letter
			int moveCount = 0;
			int i = hash(s);
	
			if(table[i] == nullptr)
			{
				table[i] = new LinkedList2();
				p = table[i];
				p->addEnd(s);
				moveCount = 0;
			}
			else
			{
				p = table[i];
				if(p->size() < HISTSIZE)
				{
					p->addEnd(s);
					moveCount = p->size()-1;
				}
				else
				{
					p->addEnd(s);
					moveCount = HISTSIZE-1;
				}
			}
				
			hist[moveCount]++;
			used++;
		}
	
		bool contains(string v) 
		{
	    	int i = 0;
	        LinkedList2* p;
	        int hashV = hash(v);
	        int numChecks = 1;

	        if(table[hashV] == nullptr)
	        {
	        	return false;
	        }
	        else
	        {
		        p = table[hashV];
		        while(p->get(i) != v)
		        {
		           if(p->get(i) == "null")
		           {
		           		return false;
		           }
		           else
		           {
		           
		           
		        	//	cout << p->get(i) << "\n";
		        		i++;
		        		numChecks++;
		           
		        	}
		        
				}
				//cout << p->get(i) << "\n";
			//	cout << "Number of hash look ups = " << numChecks << "\n";
				return true;
	        }
		}

	int getCapacity() 
	{
        return capacity;
	}
	
	int getUsed() 
	{
        return used;
	}
	
	void print()
	{
		LinkedList2* p;
		for(int i = 0; i < capacity; i++)
		{
			p = table[i];
			if(p != nullptr)
			{
				cout << i <<":";
				cout << *p << "\n";
			}
			else
			{
			//	cout << "[nullptr]\n";
			}
			
			
		}
		
		for(int j = 0; j < HISTSIZE; j++)
		{
			cout << "Hist[" << j+1 << "]= " << hist[j] << "\n";
		}
		
		cout << "Capacity: " << capacity << "\n";
		cout << "Used: " << used << "\n";
	}
		
	void printHist()
	{
		cout << "\nInsert\t\tCount \n\n";
		for(int j = 0; j < HISTSIZE -1; j++)
		{
			//cout << "Hist[" << j+1 << "]= " << hist[j] << "\n";
			cout << j+1 << "\t\t" << hist[j] << "\n";
		}
		cout << "11 or more\t" << hist[10] << "\n";
		cout << "\nBin Capacity: " << capacity << "\n";
		cout << "Words Loaded: " << used << "\n";
	}
		
	
};


void thirdTest()
{
	int answer = 0;
	HashMap myHash(50);
	int testNum = 0;
	
	string inString;
	
	myHash.add("aardvark");
	
	do{
		cout << "Enter a string\n";
		cin >> inString;

		myHash.add(inString);
		myHash.print();
		
		
		cout << "Do another?\n";
		cin >> answer;
	}while(answer != 4);
	
	do{
		cout << "Enter in a string to see if its in the hash \n";
		cin >>inString;
	
		if(myHash.contains(inString))
		{
			cout << "Contains " << inString<< "\n";
		}
		else
		{
			cout << "Does not contain " << inString << "\n";
		}
		
		cout << "Do another?\n";
		cin >> answer;
	}while(answer != 0);
//	const char* strdata = inString.c_str();

//	for (int i = 0; i < inString.length(); ++i)
    //	cout << i << strdata[i];

}

void openTestFile()
{
	int answer = 0;
	HashMap myDict(HASHSIZE);
	int testNum = 0;
	string inString;
	string inWord;
	ifstream myfile ("dict.txt");
	string::size_type sz;
    string word;
    string line;
    //int answer = 0;
    int wordCount = 0;
   
    if (myfile.is_open())
	{
    	while  (getline(myfile, line))
    	{
    		word = line.substr(0, line.find_first_of(" "));
    		myDict.add(word);
    		wordCount++;
    	}
    	cout << "Dictionary Word Count is = " << wordCount << " \n";
    	myfile.close();
	}
	else
    {
        cout << "Unable to open file";
    }
    
    //myDict.print();
    
    
    
    ifstream testfile ("hw8.dat");
    
    if (testfile.is_open())
	{
    	while  (getline(testfile, line))
    	{
    		word = line.substr(0, line.find_first_of(" "));
    		
    		if(myDict.contains(word))
    		{
    			cout << "Search Word: " << word << "\t\tis in Dictionary?: YES\n";
    		}
    		else
    		{
    			cout << "Search Word: " << word << "\t\tis in Dictionary?: NO\n";
    		}
    		
    	}
    	testfile.close();
	}
	else
    {
        cout << "Unable to open file";
    }
    
    myDict.printHist();
    
}


int main()
{
	   // firstTest();
	  // secondTest();
	 // thirdTest();
	  openTestFile();

   
    return 0;
}

