#include <iostream>

using namespace std;

int binarySearch(int x[], int length, int target)
{
	int L = 0;
	int R = length;
	int mid = -1;
	while(L < R)
	{
	    mid = (L + R) / 2;
	
    	if( x[mid] > target)
    	{
    	    R = mid - 1;
    	}
    	else if(x[mid] < target)
    	{
    	    L = mid + 1;
    	}
        else
        {
            return mid;
        }
	}
    return -1;
}

int main()
{
    int answer = 0;
    int value = 0;
    do{
        int list[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
       // int list[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
        //int list[] = { 8, 3, 4, 2, 9, 6, 5, 7, 1, 10 };
        
        const int listSize = sizeof(list)/sizeof(int);
        
        for(int i = 0; i < listSize; i++)
        {
            cout << " " << list[i] << " ";
        }
        cout << "\n";
        
        cout << "Enter in Target value to search for within the data set above \n";
        cin >> value;
        int find = binarySearch(list, listSize, value);
        
        if(find != -1)
        {
            cout << "Found the target: " << value << " at position: " << find << "\n";
        }
        else
        {
            cout << "Target: " << value << " Not Found in Data \n";
        }
        cout << "\n Run it again ? 1 = Yes, 2 = No \n";
        cin >> answer;
    }while(answer <= 1);
    return 0;
}







