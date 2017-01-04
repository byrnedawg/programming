// Name: Gregory Byrne
// Assignment: HW9 - Boggle
// https://www.quora.com/Where-can-I-get-the-source-code-for-boggle-solver-in-c++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

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

class BoggleBoard
{
private:
		int boardSize;
		int rows;
		char* boggleArray;
	
public:

	BoggleBoard(char* board, int size):boardSize(size)
	{
			rows = sqrt(boardSize);
			boggleArray = new char[boardSize];
			for(int i = 0; i < boardSize; i++)
			{
				boggleArray[i] = board[i];
			}
	}

	
	~BoggleBoard()
	{
		delete [] boggleArray;
	}

     char operator()( int i, int j ) const
     {
         return boggleArray[ i * rows + j ];
     }
     char& operator()( int i, int j ) 
     {
         return boggleArray[ i * rows + j ];
     }
	
	int getPosition(int i, int j)
	{
		return i * rows + j;
	}
	
	void printArray()
	{
		for(int i = 0; i < boardSize; i++)
		{
			cout << boggleArray[i] << " ";
		}
	}
	
	char* getBoard(char* board)
	{
		for(int i = 0; i < boardSize; i++)
		{
			board[i] = boggleArray[i];
		}
		return board;
	}
	
	char** getBoardMatrix(char** board)
	{
		for(int j = 0; j < rows; j++)
		{
			for(int i = 0; i < rows; i++)
			{
				board[j][i] = boggleArray[i+j*rows];
			}
		}
		return board;
	}
	
	bool** generateVisitedMatrix(bool** visitedMatrix)
	{
		for(int j = 0; j < rows; j++)
		{
			for(int i = 0; i < rows; i++)
			{
				visitedMatrix[j][i] = false;
			}
		}
		return visitedMatrix;
	}
	
	void printBoard()
	{
		for (int i = 0; i < rows; i++)
		{
    		for(int j = 0; j < rows; j++)
    		{ 
    	         cout << boggleArray[i * rows + j] << '\t';
    		}
    		cout << '\n';
    	}
	}
	
	char get(int i, int j)
	{
		return boggleArray[ i * rows + j ];
	}
	
	char getPosChar(int pos)
	{
		return boggleArray[pos];
	}
	
	bool isEdge(int i, int j)
	{
		if((i == 0) || (j == 0))
		{
			return true;
		}
		else if((i == rows-1) || (j == rows-1))
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	bool isEdgePos(int pos)
	{
		int cols = sqrt(boardSize);
		if((pos < cols) || (pos >= boardSize - cols)) 
		{
			return true;
		}
		else if((pos % cols == 0) || ((pos+1) % cols == 0))
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	int* getNeighborsPos(int pos, int neighborsPos[])
	{
		if(!isEdgePos(pos))
		{
			neighborsPos[0] = pos+1;
			neighborsPos[1] = pos+1+rows;
			neighborsPos[2] = pos+rows;
			neighborsPos[3] = pos-1+rows;
			neighborsPos[4] = pos-1;
			neighborsPos[5] = pos-1-rows;
			neighborsPos[6] = pos-rows;
			neighborsPos[7] = pos+1-rows;
		}
		else if(pos < rows) //top edge
		{
			if(pos == 0)
			{
				neighborsPos[0] = pos+1;
				neighborsPos[1] = pos+1+rows;
				neighborsPos[2] = pos+rows;
				neighborsPos[3] = -1;
				neighborsPos[4] = -1;
				neighborsPos[5] = -1;
				neighborsPos[6] = -1;
				neighborsPos[7] = -1;
			}
			else if(pos == rows -1)
			{
				neighborsPos[0] = -1;
				neighborsPos[1] = -1;
				neighborsPos[2] = pos+rows;
				neighborsPos[3] = pos+rows-1;
				neighborsPos[4] = pos-1;
				neighborsPos[5] = -1;
				neighborsPos[6] = -1;
				neighborsPos[7] = -1;
			}
			else
			{
				neighborsPos[0] = pos+1;
				neighborsPos[1] = pos+1+rows;
				neighborsPos[2] = pos+rows;
				neighborsPos[3] = pos+rows-1;
				neighborsPos[4] = pos-1;
				neighborsPos[5] = -1;
				neighborsPos[6] = -1;
				neighborsPos[7] = -1;
			}
			
		}
		else if(pos > (boardSize - 1 - rows)) //bottom edge
		{
			if(pos == (boardSize - rows))
			{
				neighborsPos[0] = pos+1;
				neighborsPos[1] = -1;
				neighborsPos[2] = -1;
				neighborsPos[3] = -1;
				neighborsPos[4] = -1;
				neighborsPos[5] = -1;
				neighborsPos[6] = pos-rows;
				neighborsPos[7] = pos-rows+1;
			}
			else if(pos == boardSize-1)
			{
				neighborsPos[0] = -1;
				neighborsPos[1] = -1;
				neighborsPos[2] = -1;
				neighborsPos[3] = -1;
				neighborsPos[4] = pos-1;
				neighborsPos[5] = pos-rows-1;
				neighborsPos[6] = pos-rows;
				neighborsPos[7] = -1;
			}
			else
			{
				neighborsPos[0] = pos+1;
				neighborsPos[1] = -1;
				neighborsPos[2] = -1;
				neighborsPos[3] = -1;
				neighborsPos[4] = pos-1;
				neighborsPos[5] = pos-1-rows;
				neighborsPos[6] = pos-rows;
				neighborsPos[7] = pos-rows+1;
			}
			
		}
		else if(pos % rows == 0) //left edge
		{
			neighborsPos[0] = pos+1;
			neighborsPos[1] = pos+rows+1;
			neighborsPos[2] = pos+rows;
			neighborsPos[3] = -1;
			neighborsPos[4] = -1;
			neighborsPos[5] = -1;
			neighborsPos[6] = pos-rows;
			neighborsPos[7] = pos-rows+1;
		}
		else							// right edge
		{
			neighborsPos[0] = -1;
			neighborsPos[1] = -1;
			neighborsPos[2] = pos+rows;
			neighborsPos[3] = pos+rows-1;
			neighborsPos[4] = pos-1;
			neighborsPos[5] = pos-rows-1;
			neighborsPos[6] = pos-rows;
			neighborsPos[7] = -1;
		}
		
		return neighborsPos;
	}
	
	char* getNeighbors(int i, int j, char neighbor[])
	{
		if(!isEdge(i, j))
		{
			neighbor[0] = get(i, j+1);
			neighbor[1] = get(i+1, j+1);
			neighbor[2] = get(i+1, j);
			neighbor[3] = get(i+1, j-1);
			neighbor[4] = get(i, j-1);
			neighbor[5] = get(i-1, j-1);
			neighbor[6] = get(i-1, j);
			neighbor[7] = get(i-1, j+1);
		}
		else if(i == 0)
		{
			if(j == 0)
			{
				neighbor[0] = get(i, j+1);
				neighbor[1] = get(i+1, j+1);
				neighbor[2] = get(i+1, j);
				neighbor[3] = '\0';
				neighbor[4] = '\0';
				neighbor[5] = '\0';
				neighbor[6] = '\0';
				neighbor[7] = '\0';
			}
			else if(j == rows-1)
			{
				neighbor[0] = get(i+1, j);
				neighbor[1] = get(i+1, j-1);
				neighbor[2] = get(i, j-1);
				neighbor[3] = '\0';
				neighbor[4] = '\0';
				neighbor[5] = '\0';
				neighbor[6] = '\0';
				neighbor[7] = '\0';
			}
			else
			{
				neighbor[0] = get(i, j+1);
				neighbor[1] = get(i+1, j+1);
				neighbor[2] = get(i+1, j);
				neighbor[3] = get(i+1, j-1);
				neighbor[4] = get(i, j-1);
				neighbor[5] = '\0';
				neighbor[6] = '\0';
				neighbor[7] = '\0';
			}
		}
		else if(i == rows-1)
		{
			if(j == 0)
			{
				neighbor[0] = get(i, j+1);
				neighbor[1] = get(i-1, j);
				neighbor[2] = get(i-1, j+1);
				neighbor[3] = '\0';
				neighbor[4] = '\0';
				neighbor[5] = '\0';
				neighbor[6] = '\0';
				neighbor[7] = '\0';
			}
			else if(j == rows-1)
			{
				neighbor[0] = get(i, j-1);
				neighbor[1] = get(i-1, j-1);
				neighbor[2] = get(i-1, j);
				neighbor[3] = '\0';
				neighbor[4] = '\0';
				neighbor[5] = '\0';
				neighbor[6] = '\0';
				neighbor[7] = '\0';
			}
			else
			{
				neighbor[0] = get(i, j+1);
				neighbor[1] = get(i, j-1);
				neighbor[2] = get(i-1, j-1);
				neighbor[3] = get(i-1, j);
				neighbor[4] = get(i-1, j+1);
				neighbor[5] = '\0';
				neighbor[6] = '\0';
				neighbor[7] = '\0';
			}
		}
		else
		{
			if(j == 0)
			{
				neighbor[0] = get(i, j+1);
				neighbor[1] = get(i+1, j+1);
				neighbor[2] = get(i+1, j);
				neighbor[3] = get(i-1, j);
				neighbor[4] = get(i-1, j+1);
				neighbor[5] = '\0';
				neighbor[6] = '\0';
				neighbor[7] = '\0';
			}
			else
			{
				neighbor[0] = get(i+1, j);
				neighbor[1] = get(i+1, j-1);
				neighbor[2] = get(i, j-1);
				neighbor[3] = get(i-1, j-1);
				neighbor[4] = get(i-1, j);
				neighbor[5] = '\0';
				neighbor[6] = '\0';
				neighbor[7] = '\0';
			}
		}
		return neighbor;
	}
	
	string boggleTest(int* posArray, int size)
	{
		char charArray[size];
		for(int i = 0; i < size; i++)
		{
			charArray[i] = getPosChar(posArray[i]);
		}
	
		string testWord(charArray);
		return testWord;
	}
	
	bool isPossibleWord(Trie *dict, string inWord)
	{
		return dict->startsWith(inWord);
	}
	
	bool isWord(Trie *dict, string inWord)
	{
		return dict->isWord(inWord);
	}
	
	int* generateNeighborMatrix (int* nMatrix)
	{
		int neighbors[8];
		for(int j = 0; j < boardSize; j ++)
		{
			getNeighborsPos(j, neighbors);
			for(int i = 0; i < 8; i++)
			{
				nMatrix[i+(8*j)] = neighbors[i];
			}
		}
		return nMatrix;
	}
	

bool isFirst(vector<string> *result, string inWord)
{
	if (find(result->begin(), result->end(), inWord) != result->end())
	{
		return false;
	}
	return true;
}

void solver(char **board,
          bool tracker[4][4],
          Trie *lexicon,
          string word,
          int x, int y,
          vector<string> *result)
          
  {
 
    if (lexicon->isWord(word)) 
    {
    	if(word.length() >= 3 && isFirst(result,word))
    	{
    		result->push_back(word); 
    	}
    }
 
    if (!lexicon->startsWith(word)) 
    { 
    	return; 
    }
	//bool **tmp;
	//tmp = tracker;
	
	bool tmp[4][4];
	
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < rows; j++)
		{
			tmp[i][j] = tracker[i][j];
		}
	}
	
    tmp[x][y] = true;

    //upper left
    if (0 <= x - 1 && 0 <= y - 1 && !tmp[x - 1][y - 1]){
      solver(board,tmp,lexicon, word+board[x-1][y-1], x-1, y-1, result);
    }
 
    //up
    if (0 <= y - 1 && !tmp[x][y - 1]){
      solver(board, tmp, lexicon, word + board[x][y-1], x, y-1, result);
    }
 
    //upper right
    if (x + 1 < rows && 0 <= y - 1 && !tmp[x + 1][y - 1]){
      solver(board,tmp,lexicon, word+board[x+1][y-1], x+1, y-1, result);
    }
 
    //right
    if (x + 1 < rows && !tmp[x + 1][y]){
      solver(board, tmp, lexicon, word + board[x+1][y], x+1, y, result);
    }
 
    //lower right
    if (x+1 < rows && y+1 < rows && !tmp[x+1][y+1]){
      solver(board,tmp,lexicon, word+board[x+1][y+1], x+1, y+1, result);
    }
 
    //down
    if (y + 1 < rows && !tmp[x][y + 1]){
      solver(board, tmp, lexicon, word + board[x][y+1], x, y+1, result);
    }
 
    //lower left
    if (0 <= x - 1 && y + 1 < rows && !tmp[x - 1][y + 1]){
      solver(board,tmp,lexicon, word+board[x-1][y+1], x-1, y+1, result);
    }
 
    //left
    if (0 <= x - 1 && !tmp[x - 1][y]){
      solver(board, tmp, lexicon, word + board[x-1][y], x-1, y, result);
    }
 }

	
};

int main() 
{

	Trie theDictTrie;
	ifstream myfile ("dictionary.txt");
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
    		transform(word.begin(), word.end(), word.begin(), ::tolower);
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
    //	cout << inSize <<"\n";
    	while  (getline(myfile2, line))
    	{
			transform(line.begin(), line.end(), line.begin(), ::tolower);
    		for(int i = 0; i < line.length(); i++)
    		{
    			if(i%2 == 0)
    			{
	    			inChar[j] = line[i];
	    			//cout << inChar[j] << " ";
	    			j++;
    			}
    		}
    		//cout << "\n";
    		
    	}
    	myfile2.close();
    	
    	BoggleBoard myBoard(inChar, inSize*inSize);
    
    //	myBoard.printArray();
    //	cout << "\n\n";
    //	myBoard.printBoard();
	vector<string> testWord;
	//char board1[inSize*inSize];
	//char realBoard[4][4];
	char **realBoard;
	realBoard = new char *[inSize];
	for(int i = 0; i <inSize; i++)
	{
    	realBoard[i] = new char[inSize];
	}
	
//	myBoard.getBoard(board1);
	
	myBoard.getBoardMatrix(realBoard);
	
/*
	for(int j = 0; j < inSize; j++)
	{
		for(int i = 0; i < inSize; i++)
		{
			realBoard[j][i] = board1[i+j*inSize];
		}
	}
		cout << "\n";
*/		
		for(int i = 0; i < inSize; i++)
		{
			for(int j = 0; j < inSize; j++)
			{
				cout << realBoard[i][j] << " ";
			}
			cout << "\n";
		}
		
		cout << "\n\n";

	for(int i = 0; i < inSize; i++)
	{
		for(int j = 0; j < inSize; j++)
		{
			string start = "";
			bool visited[4][4] = {{false}};
			/*
			bool **visited;
			visited = new bool *[inSize];
			for(int i = 0; i <inSize; i++)
			{
    			visited[i] = new bool[inSize];
			}
			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 4; j++)
				{
					visited[i][j] = false;
				}
			}
			*/
			myBoard.solver(realBoard, visited, &theDictTrie, start, i, j, &testWord);
		}
	}
	
	int foundWordCount = 0;
	for(int i = 0; i < testWord.size(); i++)
	{
		cout << i+1 << " " << testWord[i] << "\n";
	}
		
		foundWordCount = testWord.size();
		cout << "The number of words found is " << foundWordCount << "\n";
		
	}
	else
    {
        cout << "Unable to open file";
    }
 
  return 0;
}
