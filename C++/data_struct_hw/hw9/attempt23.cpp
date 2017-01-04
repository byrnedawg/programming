// C++ program for Boggle game
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
using namespace std;
 
#define M 3
#define N 3

class Trie 
{
private:
	class Node {
	public:
		Node* next[26];
		bool isWord;  
	
		Node()
		{ 
			next[26] = {};
			isWord = false; 
		}
	};

	Node* root;

public:

	Trie() 
	{
	  root = new Node();
	}
	
	~Trie()
	{
		delete root;
	}
	
	void add(const string& line) 
	{
		Node* node  = root;

        for(int i = 0; i < line.size(); i++){
            char x = line[i];
            if(node->next[x-'a'] == nullptr)                    
                node->next[x-'a'] = new Node();
            node = node->next[x-'a'];
        }

        node->isWord = true;
	}

	bool isWord(const string& word) 
	{  
		Node* node = root;

        for(int i = 0; i < word.size(); i++){
            char x = word[i];
            if(node->next[x-'a'] == nullptr)
                return false;
            else
                node = node->next[x-'a'];
        }

        return node->isWord;
		
	}

	// return true if the dictionary contains any words that start with these letters
	bool startsWith(const string& prefix) 
	{  
		Node* node = root;

        for(int i = 0; i < prefix.size(); i++){
            char x = prefix[i];
            if(node->next[x-'a'] == nullptr)
                return false;
            else
                node = node->next[x-'a'];
        }
        return true; 
		
	}
	
	friend void printNode(Node* node)
	{
			for(int i = 0; i < 26; i++)
			{
				Node* parent = node->next[i];
				if(parent!= nullptr)
				{	
					cout << "[" << char(i+'a') << "]-> ";
					

					for(int j = 0; j < 26; j++)
					{
						Node* child = parent->next[j];
						if(child != nullptr)
						{	
							cout << "[" << char(j+'a') << "]-> ";
							printNode(child);
						}
			
					}
				}
			}
	}
		
	
	friend ostream& operator<<(ostream& s, const Trie& trie) 
	{
	    s << "\n\t{ \n";
	    for(int i= 0; i < 26; i++)
	    {
        	if(trie.root->next[i] != nullptr)
        	{
        		cout << "\t\troot-> ";
        		cout << "[" << char(i+'a') << "]-> ";
        		printNode(trie.root->next[i]);
        		cout << " end\n";
        	}
        
	    }
		s << "\n\t}";
		return s;
	}
	
};
 
 
// Let the given dictionary be following
string dictionary[] = {"GEEKS", "FOR", "QUIZ", "GO"};
int n = sizeof(dictionary)/sizeof(dictionary[0]);
 
// A given function to check if a given string is present in
// dictionary. The implementation is naive for simplicity. As
// per the question dictionary is givem to us.
bool isWord(string &str)
{
    // Linearly search all words
    for (int i=0; i<n; i++)
        if (str.compare(dictionary[i]) == 0)
          return true;
    return false;
}
 
// A recursive function to print all words present on boggle
void findWordsUtil(Trie *dict, char boggle[M][N], bool visited[M][N], int i,
                   int j, string &str)
{
    // Mark current cell as visited and append current character
    // to str
    visited[i][j] = true;
    str = str + boggle[i][j];
    //cout << str << " ";
 
    // If str is present in dictionary, then print it
    if (dict->startsWith(str))
    {
        cout << str << endl;
    }
	    // Traverse 8 adjacent cells of boggle[i][j]
	    for (int row=i-1; row<=i+1 && row<M; row++)
	    {
	      for (int col=j-1; col<=j+1 && col<N; col++)
	      {
	        if (row>=0 && col>=0 && !visited[row][col])
	        {
	          findWordsUtil(dict, boggle,visited, row, col, str);
	        }
	      }
	    }
  
	  // Erase current character from string and mark visited
	    // of current cell as false
	    str.erase(str.length()-1);
	    visited[i][j] = false;
	
}
 
// Prints all words present in dictionary.
void findWords(char boggle[M][N], Trie *dict)
{
    // Mark all characters as not visited
    bool visited[M][N] = {{false}};
 
    // Initialize current string
    string str = "";
 
    // Consider every character and look for all words
    // starting with this character
    for (int i=0; i<M; i++)
    {
       for (int j=0; j<N; j++)
       {
             findWordsUtil(dict, boggle, visited, i, j, str);
       }
    }
}
 
// Driver program to test above function
int main()
{
    Trie theDictTrie;
	ifstream myfile ("dict.txt");
	string::size_type sz;
    string word;
    string line;
    char* wordArray;
    int answer = 0;
    int wordCount = 0;
   
    if (myfile.is_open())
	{
    	while  (getline(myfile, line))
    	{
    		word = line.substr(0, line.find_first_of(" "));
    		theDictTrie.add(word);
    		wordCount++;
    	}
    	cout << "Dictionary Word Count is = " << wordCount << " \n";
    	myfile.close();
	}
	else
    {
        cout << "Unable to open file";
    }
	
	
	int inSize;
	ifstream myfile2 ("boggle.dat");
	int j = 0;
   
    if (myfile2.is_open())
	{
    	getline(myfile2, line);
    	inSize = stoi(line, &sz);
    	char inChar[inSize*inSize];
    	cout << inSize <<"\n";
    	while  (getline(myfile2, line))
    	{
 
    		for(int i = 0; i < line.length(); i++)
    		{
    			if(i%2 == 0)
    			{
	    			inChar[j] = line[i];
	    			//cout << inChar[j] << " ";
	    			j++;
    			}
    		}
    		cout << "\n";
    		
    	}
    	myfile2.close();
    	
    	char boggle[M][N] = {{'G','I','Z'},
                        	{'U','E','K'},
                        	{'Q','S','E'}};
 
    cout << "Following words of dictionary are present\n";
    findWords(boggle, &theDictTrie);
   //cout << theDictTrie.isWord("cultch");
	}
    	
    return 0;
}