#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>

#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

int main(){

	vector<int> vec;
	vec.push_back(21);
	vec.push_back(7);
	vec.push_back(14);

	vector<int>::iterator itrBegin = vec.begin();
	vector<int>::iterator itrEnd = vec.end();

	for (vector<int>::iterator itrLoop = itrBegin; itrLoop != itrEnd; itrLoop++){
		cout << *itrLoop << endl;
	}
	//21 7 14

	list<int> integer_list;
	integer_list.push_front(1);
	integer_list.push_front(2);
	integer_list.push_front(3);
	for(list<int>::iterator list_iter = integer_list.begin(); list_iter != integer_list.end(); list_iter++){
    cout << *list_iter << endl;
	}
	// 3 2 1

	return 0;
}