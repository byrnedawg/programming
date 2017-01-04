#include <iostream>

using namespace std;

// Selection sort O(n2), bad! factor of 2 better than bubble, but can’t end early

void selectionSort(int x[], int num) 
{
    
    for (int n = num-1; n > 0; n--) 
    {
        int max = 0;
        for (int i = 1; i < n; i++)
        if (x[i] > x[max])
        {
            max = i;
        }
        int temp = x[max];
        x[max] = x[n];
        x[n] = temp;
    }
}

int main()
{
    
    int list[] = { 1, 3, 4, 2, 9, 6, 5, 7, 8, 10 };
    const int listSize = sizeof(list)/sizeof(int);

    for(int i = 0; i < listSize; i++)
    {
        cout << " " << list[i] << " ";
    }
    cout << "\n";
    
    
    selectionSort(list, 10);
    
     for(int i = 0; i < listSize; i++)
    {
        cout << " " << list[i] << " ";
    }
    
    return 0;
}
