// Name: Gregory Byrne
// Assignment: HW3b - ReadSpreadsheet

#include <iostream>
using namespace std;

int main()
{
	int row; // matrix row index
	int column; // matrix column index
	double rowSum = 0; // store sum of a row
	double totalRowSum = 0; // store sum of all rows
	double colSum = 0; // store sum of a column
	
	cout << "Enter the Row and Column of the Matrix" << endl;
	cin >> row; //enter row
	cin >> column; // enter column
	double values[row][column]; // create and array of arrays or matrix
	
	cout << "Please type doubles to fill the Matrix" << endl;

	for( int i = 0; i < row; i++ ) // nested loops to enter in values
	{
		for(int j = 0; j < column; j++)
		{
			cin >> values[i][j]; // fill matrix numbers row by row
		}
	}
	
	for(int k = 0; k < row; k++)
	{
		for(int m = 0; m < column; m++)
		{
			cout << values[k][m] << "   "; // print out row by row
			rowSum+= values[k][m];         // add up row values
		}
		cout << rowSum; // output the sum of that row
		totalRowSum += rowSum; // total the value for all the rows
		rowSum = 0; // set rowSum back to 0 for the next rowSum calculation
		cout << endl;//go to next column in the print out 
	}
	
	for(int a = 0; a< column; a++) // reverse column and rows in nested loops
	{
		for(int b = 0; b < row; b++) // reversed col and row to add columns
		{
			colSum+= values[b][a]; // reverse column rows for sums
		}
		cout << colSum << "   "; // output column sum for that column
		colSum = 0; // set colSum back to 0 for next columns calculation
	}
	cout << totalRowSum << endl; // print out the totalRowSum at the bot right
	return 0;
}