// Name: Gregory Byrne
// Assignment: HW5a - randomSort
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>


using namespace std;
									 

void quickSort(long long x[], long long left, long long right) 
{
    if (left >= right)
    {
        return;
    }
    long long pivot = (x[left] + x[right])/2;
    long long i  = left, j = right;

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
        
    }
       
   quickSort(x, left, i-1);  // logn
   quickSort(x, i, right );
   
}

void selectionSort(long long x[], long long num) 
{
    
    for (long long n = num-1; n > 0; n--) 
    {
        long long max = 0;
        for (long long i = 1; i < n; i++)
        if (x[i] > x[max])
        {
            max = i;
        }
        long long temp = x[max];
        x[max] = x[n];
        x[n] = temp;
    }
}

void insertionSort(long long x[], long long length) 
{
    long long temp = 0;
    long long j = 0;
    for(long long i=1;i<=length-1;i++)
    {
        temp=x[i];
        j=i-1;
 
        while((temp<x[j])&&(j>=0))
        {
            x[j+1]=x[j];    //moves element forward
            j=j-1;
        }
 
        x[j+1]=temp;    //insert element in proper place
    }
    
}

void printArray(const long long a[], long long n) 
{
    cout <<"{ ";
    for(long long j = 0; j < n; j++)
    {
        cout << a[j] << " ";
    }
    cout <<"}\n";
}

void sortArray(long long a[], long long n)
{
    quickSort(a, 0, n);
    printArray(a, n);
}

void printRandom(long long n, long long max)
{
    long long array[n]; 
    for(long long i= 0; i < n; i++)
    {
        long long a = rand() % max; //
        array[i] = a;
    }
    printArray(array, n);
    sortArray(array, n);
}

void printInput(long long array[], long long n)
{
   
    printArray(array, n);
    sortArray(array, n);
}

void displayTimeStamp(long long timeStamp)
{
    if(timeStamp < 10000)
    {
        cout << "The time took to Sort is: " << timeStamp << "ns\n";
    }
    else if(timeStamp < 10000000)
    {
        cout << "The time took to Sort is: " << timeStamp/1000 << "us\n";
    }
    else if(timeStamp < 10000000000)
    {
        cout << "The time took to Sort is: " << timeStamp/1000000 << "ms\n";
    }
    else
    {
        cout << "The time took to Sort is: " << timeStamp/1000000000 << " seconds\n";
    }
}
void benchmarkRandom(long long n, long long max)
{
   // struct timeval t1, t2;
  
    long long array[n]; 
    long long array1[n];
    long long array2[n];
    for(long long i= 0; i < n; i++)
    {
        long long a = rand() % max; //
        array[i] = a;
        array1[i] = a;
        array2[i] = a;
    }
    
    //gettimeofday(&t1, NULL);
    auto start = chrono::high_resolution_clock::now();
    quickSort(array, 0, n-1);
    auto finish = chrono::high_resolution_clock::now();
    //gettimeofday(&t2, NULL);
   
    
    //long double microSeconds = (t2.tv_usec - t1.tv_usec);
    
    long long timeStamp = chrono::duration_cast<chrono::nanoseconds>(finish-start).count();
    
    cout << "For a random array of size " << n << " using quick sort \n";
    displayTimeStamp(timeStamp);
    
    //cout << "The time took to Sort is: " << microSeconds << "us\n";
    
    start = chrono::high_resolution_clock::now();
    insertionSort(array1, n-1);
    finish = chrono::high_resolution_clock::now();
    
    timeStamp = chrono::duration_cast<chrono::nanoseconds>(finish-start).count();
    
    cout << "For a random array of size " << n << " using insertion sort\n";
    displayTimeStamp(timeStamp);
    
    start = chrono::high_resolution_clock::now();
    selectionSort(array2, n-1);
    finish = chrono::high_resolution_clock::now();

    timeStamp = chrono::duration_cast<chrono::nanoseconds>(finish-start).count();
    
    cout << "For a random array of size " << n << " using selection sort\n";
    displayTimeStamp(timeStamp);
}

void firstTest()
{
   
    for(int i=1; i<= 10; i++)
    {
        cout << "Random Array " << i << ": \n";
        printRandom(10, 100);
        cout << "\n";
    }
    
    cout << "Random Array: \n";
    printRandom(5, 8);
    cout << "\n";
        
    cout << "Random Array: \n";
    printRandom(10, 2);
    cout << "\n";
    cout << "Print Input = \n";
    
    long long gr[] = { 1, 3, 4, 5, 9, 11, 0, 2, 1 };
    const long long arraySize = sizeof(gr)/sizeof(long long);
    printInput(gr, arraySize);
    
    for(int i=1; i<= 100; i++)
    {
        cout << "Random Array " << i << ": \n";
        benchmarkRandom(1000,100);
        cout << "\n";
    }
    
    for(int i=1; i<= 4; i++)
    {
        cout << "Random Big Array " << i << ": \n";
        benchmarkRandom(100000, 1000000000);
        cout << "\n";
    }
}


void openTestFile()
{
	ifstream myfile ("HW5a.txt");
	//ifstream myfile ("HW5aTest.txt");
	//	ifstream myfile ("HW4c.txt");
	string::size_type sz;
    string command;
    string line;
    string inputNums;
    string numbers = "0123456789";
    string stringnum1, stringnum2;
    long long numArraySize;
    long long numMax;
    long long inputNum;
    string printRandomSTR = "PRINT RANDOM";
    string printInputSTR = "PRINT INPUT";
    string benchmarkRandomSTR = "BENCHMARK RANDOM";
   // string output2 = "INPUT\r";
 
    
    if (myfile.is_open())
	{
    	while  (getline(myfile, line))
    	{
    		command = line.substr(0, line.find_first_of(numbers.c_str())-1);
    		if(command.compare("") != 0 && command.compare("\r")!= 0 )
    		{
	    		if((command.compare(printRandomSTR) == 0))
	    		{

	    			stringnum1 = line.substr(line.find_first_of(numbers.c_str()), line.find_last_of(command) - line.find_first_of(numbers.c_str()));
	   				stringnum2 = line.substr((line.find_first_of(stringnum1)+1+stringnum1.length()));
	   				numArraySize = stoi(stringnum1, &sz);
	   				numMax = stoi(stringnum2, &sz);
	   		
	    		//	cout << "The command for this line is PRINT RANDOM " << numArraySize << " " << numMax << "\n";
		    	//	cout << "NumArraySize = " << numArraySize << "\n";
		    	//	cout << "NumMax = " << numMax << "\n";
		    	 //   cout << "stringnum1 size = " << stringnum1.length() << "\n";
		    	 //   cout << "stringnum2 size = " << stringnum2.length() << "\n";
		    	//	cout << "NumArraySize = " << stringnum1 << "\n";
		    	//	cout << "NumMax = " << stringnum2 << "\n";
		   		    printRandom(numArraySize, numMax);
		   		    cout << "\n\n";
	   			}
	   			else if((command.compare(benchmarkRandomSTR) == 0))
	    		{

	    			stringnum1 = line.substr(line.find_first_of(numbers.c_str()), line.find_last_of(command) - line.find_first_of(numbers.c_str()));
	   				stringnum2 = line.substr((line.find_first_of(stringnum1)+1+stringnum1.length()));
	   				numArraySize = stoi(stringnum1, &sz);
	   				numMax = stoi(stringnum2, &sz);
	   		
	    		//	cout << "The command for this line is BENCHMARK RANDOM " << numArraySize << " " << numMax << "\n";
		    	//	cout << "NumArraySize = " << numArraySize << "\n";
		    	//	cout << "NumMax = " << numMax << "\n";
		    	//    cout << "stringnum1 size = " << stringnum1.length() << "\n";
		    	//    cout << "stringnum2 size = " << stringnum2.length() << "\n";
		    	//	cout << "NumArraySize = " << stringnum1 << "\n";
		    	//	cout << "NumMax = " << stringnum2 << "\n";
		   		    benchmarkRandom(numArraySize, numMax);
		   		    cout << "\n\n";
	   			}
	   			else if((command.compare(printInputSTR) == 0))
	    		{

	    			stringnum1 = line.substr(line.find_first_of(numbers.c_str()), line.find_last_of(command) - line.find_first_of(numbers.c_str()));
	   			//	stringnum2 = line.substr((line.find_first_of(stringnum1)+1+stringnum1.length()));
	   				numArraySize = stoi(stringnum1, &sz);
	   				
	   		
	    		//	cout << "The command for this line is PRINT INPUT " << numArraySize << "\n";
		    	//	cout << "NumArraySize = " << numArraySize << "\n";
		    	//	cout << "NumMax = " << numMax << "\n";
		    	//    cout << "stringnum1 size = " << stringnum1.length() << "\n";
		    	//    cout << "stringnum2 size = " << stringnum2.length() << "\n";
		        //	cout << "NumArraySize = " << stringnum1 << "\n";
		    	//	cout << "NumMax = " << stringnum2 << "\n";
		   		//    printInput(numArraySize, numMax);
		   		//    cout << "\n\n";
		   		 if(numArraySize > 0)
		   		 {
    		   		 long long inputNumArray[numArraySize];
    		   		 getline(myfile, line);
    		   		 inputNums = line;
    		   		 cout << "Input values are: " << inputNums << "\n";
    		   		// cout << "The line of input numbers retrieved is " << inputNums << " \n";
    		   		 for(int i = 0; i < numArraySize; i++)
    		   		 {
    		   		     if(i < numArraySize -1)
    		   		     {
    		   		        stringnum2 = inputNums.substr(inputNums.find_first_of(numbers.c_str()), inputNums.find_first_of(' ')- line.find_first_of(numbers.c_str()));
    		   		       // cout << "Stringnum2 is " << stringnum2 << "\n"; 
    		   		        inputNum = stoi(stringnum2, &sz);
    		   		        inputNums = inputNums.substr(inputNums.find_first_of(stringnum2)+1+stringnum2.length());
    		   		        //cout << "inputNums is now " << inputNums << "\n";
    		   		     }
    		   		     else
    		   		     {
    		   		        stringnum2 = inputNums.substr(inputNums.find_first_of(numbers.c_str()), inputNums.find_first_of(' ')- line.find_first_of(numbers.c_str()));
    		   		       // cout << "Stringnum2 is " << stringnum2 << "\n"; 
    		   		        inputNum = stoi(stringnum2, &sz);
    		   		     }
    		   		     
    		   		     inputNumArray[i] = inputNum;
    		   		 }
    		   		 
    		   		 printInput(inputNumArray, numArraySize);
    		   		 cout << "\n\n";
		   		 }
		   		 else
		   		 {
		   		     cout << "PRINT INPUT command used incorrectly \n";
		   		 }
		   		    
	   			}
    	    	else
    	    	{
    	    		cout << "The Text File may be corrupt no understood commands\n\n";
    	    	}	
		    	
    		}
	    		
    	
    	}
    	myfile.close();
	}
	else
    {
        cout << "Unable to open file";
    }
}




int main() 
{
    srand(time(NULL));
//    firstTest();
	openTestFile();

  return 0;
}
