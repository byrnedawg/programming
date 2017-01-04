#include <iostream>
#include <string>
#include <vector>

using namespace std;

class HashSet {
	private:
		class LinkedList2 {
		private:
			class Node {
			public:
		    int val;
		    bool isWord;
				Node* next;
				Node(int v, Node* n) : val(v), next(n), isWord(0) {}
				Node(int v, bool end, Node* n) : val(v), next(n), isWord(end) {}
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
			
			void addWord(int v) 
			{
				if (head == nullptr) 
				{
					head = new Node(v, false, nullptr);
					tail = head;
				//	cout <<"\n The Head is pointing to " << head -> val << "\n";
				//	cout << "\n The tail is pointing to " << tail -> val << "\n";
				}
				else
				{
					tail -> next = new Node(v, false, nullptr);
					tail = tail->next;
				//	cout <<"\n The Head is pointing to " << head -> val << "\n";
				//	cout << "\n The tail is pointing to " << tail -> val << "\n";
				}
				
				count++;
			}
			
			void endOfWord(int v) 
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
						{
							cout << "LinkedList index out of bounds \n";
							return 0;
						}
			    	
			    	
			    	
			    	//	cout <<"\n The Head is pointing to " << head -> val << "\n";
					//	if(p -> next != nullptr)
					//	cout << "\n The next value is pointing to " << p->next-> val << "\n";
					//	if(tail != nullptr)
					//	cout << "\n The Tail is pointing to " << tail -> val << "\n";
				
						return p->val;
				}
			}
			
			bool isEndOfWord(int i) const 
			{ 
		
				if(i == size())
				{
					return tail->isWord;
				}
				else if(i > size())
				{
					return false;
					throw "LinkedList index out of bounds";
				}
				else
				{
					Node* p = head;
					for ( ; i > 0; i--, p = p->next)
					{
						
					}
						if (p == nullptr)
						{
							cout << "LinkedList index out of bounds \n";
							return -1;
						}
				
						return p->isWord;
				}
			}
			
			int getNextWord(int pos)
			{
				int actualPos = pos;
				if(pos == size())
				{
					if(tail->isWord)
					{
						return actualPos;
					}
					
				}
				else if(pos > size())
				{
					return -1;
					throw "LinkedList index out of bounds";
				}
				else
				{
					Node* p = head;
					for ( ; pos > 0; pos--, p = p->next)
					{
						
					}
					while(p != nullptr)
					{
						if(p->isWord)
						{
							return actualPos;
						}
						p = p->next;
						actualPos++;
					}
						
					return -1;
				}
			}
		};
		int capacity;
		int used;
		LinkedList2** table;
		
	public:
		HashSet(int cap):capacity(cap), used(0)
		{
			
			table = new LinkedList2*[capacity];
			for(int i = 0; i < capacity; i++)
			{
				table[i] = new LinkedList2();
			}
		}
		
		int hash(int v) 
		{
	        return v % capacity;
		}
	
		void add(int v) 
		{
			LinkedList2* p;
			int i = hash(v);
			p = table[i];
			p->addEnd(v);
		}
		
		void addEndOfWord(int v) 
		{
			LinkedList2* p;
			int i = hash(v);
			p = table[i];
			p->endOfWord(v);
		}
		
		void add(const string& s) 
		{
			int bin = hash(s);
		//	int count = 0;
			while (table[bin] != nullptr) 
			{
				bin = (bin+1) % capacity;
				count ++;
			}
		//	hist[count]++;
			table[bin] = new string(s);
		}
	/*
		bool contains(int v) 
		{
	    	int i = 0;
	        LinkedList2* p;
	        int hashV = hash(v);
	        int numChecks = 1;
	        p = table[hashV];
	        while(p->get(i) != v)
	        {
	           if(p->get(i) == -1)
	           {
	           		return false;
	           }
	           else
	           {
	           
	           
	        		cout << p->get(i) << "\n";
	        		i++;
	        		numChecks++;
	           
	        	}
	        
			}
			cout << p->get(i) << "\n";
			cout << "Number of hash look ups = " << numChecks << "\n";
			return true;
		}
		*/
		bool contains(int v) 
		{
	    	int i = 0;
	        LinkedList2* p;
	        int hashV = hash(v);
	        int numChecks = 1;
	        p = table[hashV];
	        while(p != nullptr)
	        {
	           if(p->get(i) == -1)
	           {
	           		return false;
	           }
	           if(p->get(i) == v)
	           {
	           		return true;
	           }
	           else
	           {
	        		cout << p->get(i) << "\n";
	        		i++;
	        		numChecks++;
	           
	        	}
	        
			}
			//cout << p->get(i) << "\n";
			//cout << "Number of hash look ups = " << numChecks << "\n";
			return false;
		}
		
		
		bool isEndOfWord2(int v) 
		{
	    	int i = 0;
	        LinkedList2* p;
	        int hashV = hash(v);
	        int numChecks = 1;
	        p = table[hashV];
	        while(i < p->size())
	        {
	           if(p->isEndOfWord(i))
	           {
	           		return true;
	           }
	           else
	           {
	        		//cout << p->isEndOfWord(i) << "\n";
	        		i++;
	        		numChecks++;
	           
	           }
	        
			}
			return false;
		}
		/*
		bool isEndOfWord2(int v)
		{
			int i = 0;
	        LinkedList2* p;
	        int hashV = hash(v);
	        //int numChecks = 1;
	        p = table[hashV];
			cout << "output of get nextword = " << p->getNextWord(i) << "\n";
		}
		*/
	
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
			cout << *p << "\n";
			
		}
	}
		
		
	
};



void secondTest()
{
	HashSet myHash(5);
	int testNum = 0;
	
	myHash.add(7);
	myHash.add(203);
	myHash.add(26);
	myHash.add(56);
	myHash.add(12);
	myHash.add(75);
	myHash.add(45);
	myHash.add(246);
	myHash.add(26);
	myHash.addEndOfWord(31);
	myHash.add(714);
	myHash.addEndOfWord(714);
	myHash.add(405);
	myHash.add(26);
	myHash.add(206);
	myHash.add(981);
	myHash.add(757);
	myHash.add(425);
	myHash.add(296);
	myHash.add(926);
	myHash.add(123);
	myHash.print();
	
	cout << "\n";
	do{
		cout << "Enter in a number to test if its in the hash \n";
		cin >>testNum;
		cout << "Searching ... \n\n";
		if(myHash.contains(testNum))
		{
			cout << "Hash contains " << testNum << "\n";
		}
		else
		{
			cout << "Hash Does not contain " << testNum << "\n";
		}
	
	}while(testNum != 0);
	
	do{
		cout << "Enter in a number to test if its the end of a word \n";
		cin >>testNum;
	
		if(myHash.isEndOfWord2(testNum))
		{
			cout << "Is the end " << testNum << "\n";
		}
		else
		{
			cout << "Is not the end " << testNum << "\n";
		}
		
	}while(testNum != 0);
	
}

void thirdTest()
{
	int answer = 0;
	HashSet myHash(26);
	int testNum = 0;
	   
	LinkedList2* p;
	string inString;
	char inChar;
	do{
		cout << "Enter a string\n";
		cin >> inString;
		
		myHash.add(inString);
		
		/*
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
		*/
		myHash.print();
		
		
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
	  // secondTest();
	   thirdTest();

   
    return 0;
}
