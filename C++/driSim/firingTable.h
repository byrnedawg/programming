#ifndef SHELLTABLE_H_
#define SHELLTABLE_H_

#include <stdio.h>
#include <string.h>

extern int gblcharge;
extern int fztime;	/* fuze time for this particular shell and elev in .1 sec units */

extern const char * ammotype[5][6];

extern const char * group[5];

struct lookup
{
    const int meters; 
    const int mils; 
};

struct FTpoint 
{ 
	const char * shelltype ;
	const struct lookup * firstch0;	/* points to first lookup in the charge 0 table for this shelltype */
	const struct lookup * lastch0;
	const struct lookup * firstch1;
	const struct lookup * lastch1;
	const int * fuzetimes;			/* either null pointer or points to array a fuzetimes .1 sec units */
};


int calculateRange(int elv, const char * shltyp, int charge);

int calculateElevation(int distance, const char * shltyp, int charge);

int calculateElevationVert(int distance, const char * shltyp, int charge, int vert);


#endif 