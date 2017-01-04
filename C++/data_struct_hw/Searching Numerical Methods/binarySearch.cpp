#include <iostream>

using namespace std;

int binarySearch(int x[], int length, int target)
{
	int L = 0;
	int R = length - 1;
	int mid = -1;
	while(L < R)
	{
	    mid = (L + R) / 2;
	    cout << "L = " << L << " mid = " << mid << " R = " << R << "\n";
	
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

int binSearch(int x[], int a, int b, int target)
{
	if(a == b)
	{
	    return -1;
	}
	int guess = (a + b)/2;
	if(x[guess] < target)
	{
	    return binSearch(x, target, guess+1, b);
	}
	else if(x[guess] > target)
	{
	    return binSearch(x, target, a, guess-1);
	}
	else
	{
	    return guess;
	}
}

int binary_search2(int x[], int left, int right, int target) 
{
    if (left > right) {
        return -1;
    }

    int mid = (left + right) / 2;
     cout << "L = " << left << " mid = " << mid << " R = " << right << "\n";

    if (x[mid] == target) {
        return mid;
    }

    if (x[mid] > target) {
        return binary_search2(x, left, mid - 1, target);
    }

    return binary_search2(x, mid + 1, right, target);
}

int main()
{
    int answer = 0;
    int value = 0;
    do{
        //int list[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int list[] = { 1, 3, 4, 6, 7, 7, 8, 9};
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
        //int find = binarySearch(list, listSize, value);
        //int find = binSearch(list, 0, listSize, value);
        int find = binary_search2(list, 0, listSize-1, value);
        
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







