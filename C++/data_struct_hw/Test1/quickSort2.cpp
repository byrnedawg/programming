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

  /* partition */
    while (i <= j) 
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
    }
    /* recursion */
   
  quickSort(x, left, i-1);
  quickSort(x, i, right);
  
   // if (left < j)
   // {
       // quickSort(x, left, j);
  //  }
   // if (right > i)
   // {
      //  quickSort(x, i, right);
   // }

}

int main()
{
    
    //int list[] = { 1, 3, 4, 2, 9, 6, 5, 7, 8, 10 };
    int list[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
    const int listSize = sizeof(list)/sizeof(int);
    cout << "List is of size " << listSize << "\n";

    for(int i = 0; i < listSize; i++)
    {
        cout << " " << list[i] << " ";
    }
    cout << "\n";
    
    
    quickSort(list, 0, listSize -1);
    
     for(int i = 0; i < listSize; i++)
    {
        cout << " " << list[i] << " ";
    }
    
    return 0;
}