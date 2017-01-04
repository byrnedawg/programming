// Name: Gregory Byrne

#include <iostream>
#include <cmath>
using namespace std;

void printArray(const double g[], const int n) 
{
	for (int i = 0; i < n; i++)
	{
		cout << i << ":" << g[i] << endl;
	}
	cout << endl;
}

void stats(const double g[], int length, double& mean, double& max, double& min, double& variance, double& std) 
{
	double varSum = 0;
	mean = g[0];
	max = g[0];
	min = g[0];
	
	for( int i = 1; i < length; i++ )
	{
		mean += g[i];
		
		if(g[i] > max)
		{
			max = g[i];
		}
		if(g[i] < min)
		{
			min = g[i];
		}
	}
	
	mean = mean / length;
	
	for(int j = 0; j < length; j++)
	{
		varSum += pow((g[j] - mean), 2);
	}
	
	variance = varSum / length;
	std = sqrt(variance);
}

int main()
{
	int index;
	double meanValue;
	double maxValue;
	double minValue;
	double varianceValue;
	double stdValue;
	
	cout << "Enter the Size of the Array" << endl;
	cin >> index;
	double values[index];
	
	cout << "Please type doubles to fill Array" << endl;

	for( int i = 0; i < index; i++ )
	{
		cout << "Number " << i + 1 << " : ";
		cin >> values[i];
	}
	
	printArray(values, index);
	stats(values, index, meanValue, maxValue, minValue , varianceValue, stdValue);
	
	cout << "The mean of these numbers is = " << meanValue << endl;
	cout << "The max of these numbers is = " << maxValue << endl;
	cout << "The min of these numbers is = " << minValue << endl;
	cout << "The variance of these numbers is = " << varianceValue << endl;
	cout << "The Standard Deviation of these numbers is = " << stdValue << endl;

	return 0;
}