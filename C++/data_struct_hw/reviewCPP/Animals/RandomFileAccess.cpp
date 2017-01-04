#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main(){

	FILE* fPointer;
	// w+ means write it first and than read it
	fPointer = fopen("RandomFileAccess.txt", "w+");
	fputs("I ate 3 pumpkins today", fPointer);
	// SEEK_SET = From the begining of the file
	// SEEK_CUR = From the current position of the file
	// SEEK_END = From the end of the file
	fseek(fPointer, 7, SEEK_SET);
	fputs(" munchikins on Friday", fPointer);
	// -> I ate 3 munchikins on Friday
	fseek(fPointer, -6, SEEK_END);
	fputs("top of a mountain", fPointer);
	// -> I ate 3 munchikins on top of a mountain

	// BINARY
	double value;
	char* pmemory = (char*) &value;

	// Open binary file to write:
	// ios::app   -- Append to the file
	// ios::ate   -- Set the current position to the end
	// ios::trunc -- Delete everything in the file
	ofstream out_file("RandomFileAccess.bin", ios::out | ios::binary | ios::trunc);
	while (cin >> value) {
		out_file.write(pmemory, sizeof(double));
	}
	out_file.close();

	// Open binary file to read:
	ifstream in_file("RandomFileAccess.bin", ios::in | ios::binary);
	in_file.read(pmemory, sizeof(double));

	while (!in_file.eof()) {
			cout << value << endl;
			in_file.read(pmemory, sizeof(double));
	}
	in_file.close();

	return 0;
}