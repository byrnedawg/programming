#include <iostream>

using namespace std;

/* O(n log n)

Worst case (pathological) O(n2)

*/

void quickSort(int x[], int left, int right) 
{
    if (left >= right)
    {
        return;
    }
    int pivot = (x[left] + x[right])/2;
    int i  = left, j = right;

    while (i <= j)		//O(n)
    {
        while (x[i] < pivot)
        {
            i++;
        }
        while (x[j] > pivot)
        {
            j--;
        }
        if (i <= j) 
        {
            swap(x[i], x[j]);
             i++;
             j--;
        }
        
        for(int k = 0; k < 8; k++)
        {
            cout << " " << x[k] << " ";
        }
        cout << "\n";
    }
   
   //i == j
   quickSort(x, left, i-1);  // logn
   quickSort(x, i, right );
   
}


int main()
{
    //int list[] = { 8, 7, 5, 6, 3, 4, 2, 1};
    int list[] = { 1, 3, 9, 6, 8, 6, 2, 4};
    
    //int list[] = { 1, 3, 9, 6, 8, 6, 2, 4, 8, 10, 7 };
    //int list[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
    const int listSize = sizeof(list)/sizeof(int);
    cout << "List is of size " << listSize << "\n";

    for(int i = 0; i < listSize; i++)
    {
        cout << " " << list[i] << " ";
    }
    cout << "\n\n";
    
    
    quickSort(list, 0, listSize -1);
    
     for(int i = 0; i < listSize; i++)
    {
        cout << " " << list[i] << " ";
    }
    
    return 0;
}





















