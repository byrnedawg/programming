// Name: Gregory Byrne
// Assignment: HW3f - Array statistics
#include <iostream>
using namespace std;

void printGrades(const int g[], int n) 
{
    for (int i = 0; i < n; i++)
	{
		cout << i << ":" << g[i] << endl; // print grades one at a time
	}
	cout << endl;
}

double calcmean(const int g[], int length) 
{
    double mean = g[0];
    for( int i = 1; i < length; i++ )
	{
		mean += g[i]; //sum up all the grades
	}
    return mean / length; //return the sum / num grades
}

void stats(const int g[], int length, double& mean, int& max, int& min) 
{
	mean = g[0]; // store mean
	max = g[0]; // store max
	min = g[0]; // store mean
	
	for( int i = 1; i < length; i++ ) //start at 1 since variables are intitalized with g[0]
	{
		mean += g[i]; // add up total sum of grades
		
		if(g[i] > max) // see if the next value is larger
		{
			max = g[i]; // update max
		}
		if(g[i] < min) // see if the next value is smaller
		{
			min = g[i]; // update min
		}
	}
	
	mean = mean / length; // pass mean by reference sum / num of grades
}

int main() 
{
	int gr[] = { 95, 80, 90, 65, 93 }; // grade array
	const int n = sizeof(gr)/sizeof(int); //size of array
	double mean; // value for mean
	int max, min; // value for max and min
	stats(gr, n, mean, max, min); // get stats all values passed by reference
	printGrades(gr, n); // print our the grades in the array
	cout << "The max grade is " << max << endl;
	cout << "The min grade is " << min << endl;
	cout << "The mean of the grades is " << mean << endl;
	cout << "The mean calculated with its own function = " << calcmean(gr, n) << '\n';
	
    return 0;
}

