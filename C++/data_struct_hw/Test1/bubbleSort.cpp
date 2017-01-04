#include <iostream>

using namespace std;

/*
n - 1 comparisons		+ 	n - 1 swaps = O(n)

n-1 + (n-2) + (n-3) + … 1 = n (n-1) / 2 = O(n2)
*/

void bubbleSort(int x[], int n) 
{
   for (int j = 0; j < n-1; j++) 
   {
        for (int i = 0; i < n-1-j; i++)
        {
            if (x[i] > x[i+1]) 
            {
                int temp = x[i];
                x[i] = x[i+1];
                x[i+1] = temp;
            }
        }
   }
}

//O(n2)   Ω(n)
void improvedBubbleSort(int x[], int n) 
{
    for (int j = 0; j < n-1; j++) 
    {
        bool sorted = true;
        for (int i = 0; i < n-1-j; i++)
        {
            if (x[i] > x[i+1]) 
            {
                int temp = x[i];
                x[i] = x[i+1];
                x[i+1] = temp;
                sorted = false;
            }
        }
        if (sorted)
        {
            return;
        }
        
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
    
    //bubbleSort(list, 10);
    improvedBubbleSort(list, 10);
    
     for(int i = 0; i < listSize; i++)
    {
        cout << " " << list[i] << " ";
    }
    
    return 0;
}
