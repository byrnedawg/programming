#include <iostream>

using namespace std;

void slowshuffle(int x[], int length)
{
	int y[length];
	for(int i = 0; i < length; i++)
	{
        int pick = 0;
    	do
    	{
    		pick = rand() % length;
    	}while(x[pick] < 0);
    	y[i] = x[pick];
    	x[pick] = -1;
    	swap(x[i], x[pick]);
    	
	}

	for(int j = 0; j < length; j++)
	{
	    x[j] =y[j];
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
        
        
        slowshuffle(list, listSize);
        
         for(int i = 0; i < listSize; i++)
        {
            cout << " " << list[i] << " ";
        }
        
        cout << "\n Run it again ? 1 = Yes, 2 = No \n";
        cin >> answer;
    }while(answer <= 1);
    return 0;
}







