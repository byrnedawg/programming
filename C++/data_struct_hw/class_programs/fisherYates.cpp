#include <iostream>

using namespace std;

void fisherYates(int x[], int length)
{
	for(int i = length-1 ; i > 0; i--)
	{
	    int pick = rand() % i;
		swap(x[i], x[pick]);
    }
}


int main()
{
    int answer = 0;
    do{
        int list[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        const int listSize = sizeof(list)/sizeof(int);
    
        for(int i = 0; i < listSize; i++)
        {
            cout << " " << list[i] << " ";
        }
        cout << "\n";
        
        
        fisherYates(list, listSize);
        
         for(int i = 0; i < listSize; i++)
        {
            cout << " " << list[i] << " ";
        }
        
        cout << "\n Run it again ? 1 = Yes, 2 = No \n";
        cin >> answer;
    }while(answer <= 1);
    return 0;
}







