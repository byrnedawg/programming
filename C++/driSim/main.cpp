#include <iostream>

#include "firingTable.h"

using namespace std;

void commandLineTest()
{
	
	int answer = 0;
	int value = 0;
	int roundType = 0;
	int roundName = 0;
	int charge = 0;
	gblcharge = charge;
	int calcRange = 0;
	int elevation = 0;
	int calcElv = 0;
	
	
	cout << "-----------------------------------------------------------\n";
	cout << "DRI Simulator V1.0 \n";
	cout << "1: Enter in Distance in Meters \n";
	cout << "2: Enter Elevation in Mils \n";
	cout << "3: Select ammo type \n";
	cout << "4: Select Munition \n";
	cout << "5: Select Charge 0 or 1 \n";
	cout << "6: Display Status of gun \n";
	cout << "7: Display fun \n";
	cout << "9: Exit Simulator \n";
	cout << "-----------------------------------------------------------\n";		

	do{
		cin >> answer;
		if(answer == 1)
		{
			cout << "Enter Distance in Meters \n";
			cin >> value;
			cout << "Distance " << value << " is entered \n\n";
			calcElv = calculateElevation(value,(char*)(ammotype[roundType][roundName]), gblcharge);
			if(calcElv == -1)
			{
				cout << "Elevation: TOO CLOSE \n";
			}
			else if(calcElv == -2)
			{
				cout << "Elevation: TOO FAR \n";
			}
			else
			{
				cout << "Elevation: " << calcElv << " Mils \n";
				elevation = calcElv;
			}
		}
		else if(answer == 2)
		{
			cout << "Enter Elevation in Mils \n";
			cin >> elevation;
			cout << "Elevation " << elevation << " is entered \n\n";
			calcRange = calculateRange(elevation,(char*)(ammotype[roundType][roundName]), gblcharge);
			if(calcRange == -1)
			{
				cout << "Range: TOO HIGH \n";
			}
			else if(calcRange == -2)
			{
				cout << "Range: TOO LOW \n";
			}
			else
			{
				cout << "Range: " << calcRange << " Meters \n";
			}
		}
		else if(answer == 3)
		{
			cout << "Enter Ammo Type \n";
			cout << "1: HE/MAPAM \n2: WP \n3: Illum \n4: Train \n5: Elevation \n";
			cin >> roundType;
			roundType--;
			cout << "Ammo Type " << roundType << " is entered \n\n";
			cout << "The Ammo Type selected is " << group[roundType] << 
			" with first munition named " << ammotype[roundType][0] << "\n" ;
		}
		else if(answer == 4)
		{
			cout << "Select Munition in round type " << group[roundType] << "\n";
			for(int i = 0; i < 5; i++)
			{
				if(strcmp(ammotype[roundType][i],"d") != 0)
				{
					cout << i+1 << ":" << ammotype[roundType][i] << "\n";
				}
			}
			cin >> roundName;
			cout << "Round Name " << roundName << " is entered \n\n";
			roundName--;
			cout << "The ammo type selected is " << ammotype[roundType][roundName] << "\n" ;
		}
		else if(answer == 5)
		{
			cout << "Select Charge 0 or 1 for \nRound Type: " << group[roundType] << "\nMuniton: " << ammotype[roundType][roundName] << "\n";
			cin >> charge;
			if(charge > 4 || charge < 0)
			{
				charge = 0;
			}
			gblcharge = charge;
			cout << "Charge " << charge << " is Selected\n\n";
			cout << "The current charge selected is " << charge << "\n" ;
		}
		else if(answer == 6)
		{
			cout << "Gun Status\n";
			calcRange = calculateRange(elevation,(char*)(ammotype[roundType][roundName]), gblcharge);
			cout << "Round Type: " << group[roundType] << "\n";
			cout << "Muniton: " << ammotype[roundType][roundName] << "\n";
			cout << "Charge: " << gblcharge << "\n" ;
			cout << "Tube Elevation: " << elevation << "\n";
			if(calcRange == -1)
			{
				cout << "Range: TOO HIGH \n";
			}
			else if(calcRange == -2)
			{
				cout << "Range: TOO LOW \n";
			}
			else
			{
				cout << "Range: " << calcRange << " Meters \n";
			}
			if(strcmp(group[roundType],"Illum") == 0)
			{
				cout << "Fuze Time: " << fztime << " Seconds \n";
			}
		}
		else if(answer == 7)
		{
			//cout << "Display Fun: " << firstFT->shelltype << "\n" ;
			//first = firstFT->firstch1;
			//cout << "Display mils: " << first->mils << "\n" ;
		}
		else
		{
			cout << "-----------------------------------------------------------\n";
			cout << "DRI Simulator V1.0 \n";
			cout << "1: Enter in Distance in Meters \n";
			cout << "2: Enter Elevation in Mils \n";
			cout << "3: Select ammo type \n";
			cout << "4: Select Munition \n";
			cout << "5: Select Charge 0 or 1 \n";
			cout << "6: Display Status of gun \n";
			cout << "7: Display fun \n";
			cout << "9: Exit Simulator \n";
			cout << "-----------------------------------------------------------\n";
		}
		
	}while(answer != 9);
	
}
int main(void)
{
	while(1)
	{
		commandLineTest();
		return 0;
	}
}

