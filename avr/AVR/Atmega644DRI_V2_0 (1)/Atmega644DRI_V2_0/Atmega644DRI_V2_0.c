/*
 * Atmega644DRI_V2_0.c
 *
 * Created: 12/9/2015 1:32:50 PM
 *  Author: ByrneDawg
 */ 

#define F_CPU 8000000UL
#include <stdlib.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <math.h>
#include "i2c.h"
#include "lcd_lib.h"
#include "Sensor.h"
#include "program_Memory.h"
#include "print2LCD.h"

// MIN/MAX/ABS macros
#define MIN(a,b)			((a<b)?(a):(b))
#define MAX(a,b)			((a>b)?(a):(b))
#define ABS(x)				((x>0)?(x):(-x))

// constants
#define PI		3.14159265359

//Port Define Helpers
#define output_low(port,pin) port &= ~(1<<pin)		// output_low(PORTB, PB2);
#define output_high(port,pin) port |= (1<<pin)		// output_high(PORTB, PB2);
#define output_toggle(port,pin) port ^= (1<<pin)	// output_toggle(PORTB, PB2);

#define set_input(portdir,pin) portdir &= ~(1<<pin)		//set_input(DDRB, PB2);
#define set_output(portdir,pin) portdir |= (1<<pin)		//set_output(DDRB, PB2);

#define is_set(portpin,pin) (portpin & (1<<pin)) == (1<<pin)          //(for input) checks if the ... if(is_set(PINB,PB0)) input is high (Logic 1)
#define is_clear(portpin,pin) (portpin & (1<<pin)) == (0<<pin)		//(for input) checks if the ... if(is_clear(PINB,PB0)) input is low (Logic 0)

#define Switch1 PA0
#define Switch2 PD5
#define Switch3 PD6
#define GreenLED PD7
#define PowerOn PA2
#define PowerSwitch PA1

// i2C Stuff
#define MMA8452_ADDRESS 0x1D
#define FALSE	0
#define TRUE	-1
const uint8_t SCALE = 2;  // Sets full-scale range to +/-2, 4, or 8g. Used to calc real g values.
// Set the output data rate below. Value should be between 0 and 7
const uint8_t dataRate = 0;  // 0=800Hz, 1=400, 2=200, 3=100, 4=50, 5=12.5, 6=6.25, 7=1.56

int accelCount[3];  // Stores the 12-bit signed value
float accelG[3];  // Stores the real accel value in g's
float accelD[3];  // Stores the real accel value in degrees
float accelM[3];  // Stores the real accel value in mils
float accel3Axis[3];
float buns = 1.75;

//i2C Stuff

volatile uint16_t timer0Overflows = 0; // Stores the value for timer polling timer 0
volatile uint8_t switch_state = 8;	// Switch state
volatile uint8_t power_count = 0;
volatile int16_t i = -1024;
volatile int16_t d = 1;
volatile uint8_t BTN_Down = 0;
volatile uint8_t updateSpeed = 32;
volatile uint32_t system_Clock = 0;
volatile uint8_t z = 0;
volatile int angle=0; 
volatile int range=0;
volatile uint8_t gblcharge;
volatile uint8_t fztime;	/* fuze time for this particular shell and elev in .1 sec units */

//uint8_t numberDisplay[17]=" ";
uint8_t Buffer[1000];
char rangestring[5]="00000";
char fuzestring[4];
uint8_t index = 0;

char outstring[17];	/* line buffer ...null terminated since ram cleared used for LCD output*/
char ostr2[17];     /* a second output string to test...so both lines out together*/
//int lcdposition = 0 ;		/* counter used to go between 1st and 2nd line of the LCD display*/



static  struct ramcopy{  /* the ram copy of the restarting struct*/
	int groupnum;
	int ammonum;
	int chargenum;
	int anglecorrect;
} ramcalstr ;

struct ramcopy *ramstrp = &ramcalstr;

const char * toolow = "    TOO LOW     ";
const char * toohigh = "    TOO HIGH    ";

/*** ammo types are grouped HE/MAPAM, WP, Illum, Training  ***/

const char * ammotype[][6] = {{"M720A1","M720","M1061",
"M768","M888","d"},


{"M722A1","M722","d","d","d","d"},

{"M721","d","d","d","d","d"},

{"M769","d","d","d","d","d"},
{"ELEV","d","d","d","d","d"}};

const char * charge[] = {"0","1"};
const char * group[]  = {"HE/MAPAM","WP","Illum","Train","Elevation"};
const char* setcharge = "select charge   " ;
const char* selammo = "ammo type       " ;
const char* selgroup = "ammo group      " ;
const int ammolim[] = {4,1,0,0,0};
int vectflag;
char * charptr;
char * tptr;
char * fptr;
char elev[5];	/* elevation in mils ascii decimal...trailing null */
char countdnstr[3];

int countdntime;	/* timer for last 15 seconds before power off */

int ammonum = 0;
int chargenum = 0;
int groupnum = 0;

void update_Display(void) //
{
	if(switch_state == 0 ) //
	{
		CopyStringtoLCD(State0, 3, 0);
		printInt(11, 0 , i);
		printInt(0, 1 , system_Clock);
		LCDGotoXY(5,1);
		lcd_puts(" GREG BYRNE");
		output_low(PORTD, GreenLED);
		updateSpeed = 32;
		
	}
	if(switch_state == 1)	//Red
	{
		CopyStringtoLCD(State1, 3, 0);
		printInt(11, 0 , i);
		output_high(PORTD, GreenLED);
		updateSpeed = 16;
	}
	else if(switch_state == 2)	//Orange
	{
		CopyStringtoLCD(State2, 3, 0);
		printInt(11, 0 , i);
		printTime(3,1, system_Clock);
		output_low(PORTD, GreenLED);
		updateSpeed = 8;
	}
	else if(switch_state == 3)	//Yellow
	{
		CopyStringtoLCD(State3, 3, 0);
		LCDGotoXY(0,1);
		LCDsendChar('X');
		printAccelDeg(1, 1 , (int)accelD[0]);
		LCDGotoXY(9,1);
		LCDsendChar('Y');
		printAccelDeg(10, 1 , (int)accelD[1]);
		output_high(PORTD, GreenLED);
		updateSpeed = 4;
	}
	
	else if(switch_state == 4)	//Green
	{
		CopyStringtoLCD(State4, 3, 0);
		LCDGotoXY(0,1);
		LCDsendChar('X');
		printAccelMil(1, 1 , (int)accelM[0]);
		LCDGotoXY(9,1);
		LCDsendChar('Y');
		printAccelMil(10, 1 , (int)accelM[1]);
		//LCDGotoXY(12,1);
		//LCDsendChar('Z');
		//printAccel(13, 1 , (int)accelD[2]);
		
		output_low(PORTD, GreenLED);
		//updateSpeed = 2;
	}
	else if(switch_state == 5)	//Blue
	{
		CopyStringtoLCD(State5, 3, 0);
		printInt(11, 0 , i);
		//printInt(5, 1 , system_Clock);
		sprintf(rangestring,"%5d",(int)accelM[0]);
		/* convert leading zeros to leading blanks */
		index = 0;
		while(rangestring[index] == '\0') {rangestring[index] = ' ' ; index++;}
		LCDGotoXY(0,1);
		LCDsendChar('X');
		for(index = 0; index < 5; index++)
		{
			LCDsendChar(rangestring[index]);
		}
		sprintf(rangestring,"%5d",(int)accelM[1]);
		/* convert leading zeros to leading blanks */
		index = 0;
		while(rangestring[index] == '\0') {rangestring[index] = ' ' ; index++;}
		LCDGotoXY(9,1);
		LCDsendChar('Y');
		for(index = 0; index < 5; index++)
		{
			LCDsendChar(rangestring[index]);
		}
		output_high(PORTD, GreenLED);
		//updateSpeed = 1;
	}
	else if(switch_state == 6)	//Blue
	{
		CopyStringtoLCD(State6, 3, 0);
		printInt(11, 0 , i);
		LCDGotoXY(0,1);
		for(z = 0; z < 16; z++) outstring[z] = ' ';
		outstring[16]= '\0';
		charptr = (char *)group[groupnum];
		tptr = &outstring[0];
		while (*charptr) *tptr++ = *charptr++ ; /* COPY Group STRING TO OUTSTRING */
		lcd_puts(outstring);
		output_low(PORTD, GreenLED);
		updateSpeed = 64;
	}
	else if(switch_state == 7)	//Blue
	{
		CopyStringtoLCD(State7, 3, 0);
		printInt(11, 0 , i);
		LCDGotoXY(0,1);
		for(z = 0; z < 16; z++) outstring[z] = ' ';
		outstring[16]= '\0';
		charptr = (char *)ammotype[groupnum][ammonum];
		tptr = &outstring[0];
		while (*charptr) *tptr++ = *charptr++ ; /* COPY AMMO TYPE STRING TO OUTSTRING */
		lcd_puts(outstring);
		output_high(PORTD, GreenLED);
		updateSpeed = 8;
	}
	else if(switch_state == 8)	//Blue
	{
		//angle = (int)accelM[1];
		//angle = (int)accelM[0];
		//angle = 1297;
		//sscanf(elev,"%d",&angle);
		//sprintf(elev,"%4d",angle);
		ostr2[16]= '\0';
		//sprintf(elev,"%5d",(int)accelM[0]); //single axis reading
		sprintf(elev,"%5d",(int)accel3Axis[0]); // 3 axis reading
		fptr = elev;
		tptr = &ostr2[0];
		//tptr = &outstring[5];
		for(z = 0; z < 5 ; z++) *tptr++ = *fptr++;
		//LCDGotoXY(10,1);
		 //pgm_read_ptr(&(ammotype[groupnum][ammonum]));
		 		 
		 
		 
		if(groupnum == 4)
		{
			LCDGotoXY(5,0);
			lcd_puts(ostr2); /* IS elevation mode just put out elevation on 1st line */
		}
		else 
		{
			//range = trans((int)accelM[0],(char *)ammotype[groupnum][ammonum],chargenum); // single axis reading
			range = trans((int)accel3Axis[0],(char *)ammotype[groupnum][ammonum],chargenum); // 3 axis reading
			//range = trans((int)accelM[0],(char*)pgm_read_word(ammotype[groupnum][ammonum]),chargenum);
			chargenum = gblcharge;	/* change charge if ammotype only supports charge 1 */
			/* check for too high or too low returns first */
			//	if(range == -1) for(i = 0; i < 16; i++) outstring[i] = toohigh[i];

			/*** changed 24 March 2011...here checking also for past vertical the == '-' part **/
			if(range == -1)
			{
				for(z = 0; z < 16; z++)
				{
				outstring[z] = toohigh[z];
				} 
				LCDGotoXY(0,0);
				lcd_puts(outstring);
			}
			else if(range == -2)
			{
				for(z = 0; z < 16; z++) 
				{
					outstring[z] = toolow[z];
				}
				LCDGotoXY(0,0);
				lcd_puts(outstring);
			}
			else /* is in range */ 
			{	
				sprintf(rangestring,"%4d",range);
				/* convert leading zeros to leading blanks */
				z = 0;
				while(rangestring[z] == '\0') {rangestring[z] = ' ' ; z++;}
				if(fztime) sprintf(fuzestring,"%3d",fztime);
				/* now assemble the output buffer */
				for(z = 0; z < 16; z++) outstring[z] = ' ' ;
				for(z = 0; z < 4; z++) outstring[z + 5] = rangestring[z];
				outstring[10] = 'm';	/* unit is meters */
				//if(fztime)
				if(!strcmp(group[groupnum],"Illum"))
				{
					outstring[12] = fuzestring[0];
					outstring[13] = fuzestring[1];
					outstring[14] = '.';
					outstring[15] = fuzestring[2];
				}
				LCDGotoXY(0,0);
				lcd_puts(outstring);
			}
			
		}
		
		for(z = 0; z < 16; z++) outstring[z] = ' ' ; /* 17th character already null */
		outstring[16]= '\0';
		/* overwrite start of outstring with ammo type and charge */
		fptr = (char *)ammotype[groupnum][ammonum];
		tptr = outstring;
		while(*fptr)(*tptr++ = *fptr++);
		fptr = (char *)charge[chargenum];
		tptr = &outstring[7];
		while (*fptr)(*tptr++ = *fptr++);
		//for(z = 9; z < 16; z++)
		outstring[9] = ostr2[0];
		outstring[10] = ostr2[1];
		outstring[11] = ostr2[2];
		outstring[12] = ostr2[3];
		outstring[13] = ostr2[4];
		outstring[14] = ostr2[5];
		outstring[15] = ostr2[6];
		LCDGotoXY(0,1);
		lcd_puts(outstring);
		
		//printAccelMil(11, 1 , (int)accelM[0]);
					
	}
	else if(switch_state == 9)	//Yellow
	{
		CopyStringtoLCD(State9, 3, 0);
		LCDGotoXY(0,1);
		LCDsendChar('T');
		printAccelTilt(1, 1 , (int)accel3Axis[0]);
		LCDGotoXY(9,1);
		LCDsendChar('R');
		printAccelTilt(10, 1 , (int)accel3Axis[1]);
		output_low(PORTD, GreenLED);
	}
	else
	{
		CopyStringtoLCD(State0, 3, 0);
		printInt(11, 0 , i);
		printInt(0, 1 , system_Clock);
		output_low(PORTD, GreenLED);
		updateSpeed = 32;
		switch_state = 0; // set switch state to 0 if the state is above 9
	}

}

ISR(TIMER0_OVF_vect) // timer 0 overflow interrupt
{
	//TCNT0 += 6;
	TCNT0 += 0x83;
	if((timer0Overflows % 2) == 0) // push button polling 195 1 sec /975 5 sec/ 11700 1 min/
	{
		if(is_clear(PINA, Switch1)) // if the push button is held down connecting pin B2 to ground
		{
			BTN_Down = 1; // button down is true
		}
	}
	if((timer0Overflows % 4) == 0) // push button polling on the release
	{

		if(is_set(PINA, Switch1) && BTN_Down == 1) //
		{
			//switch_state++; // increment mode states by 1 i.e. red -> yellow, green - > blue
			if (switch_state == 6)
			{
		
				if (groupnum >= 4) 
				{
					groupnum = 0;
				}
				else 
				{
					groupnum++;
				}
			}
			if (switch_state == 7)
			{
				if (ammonum >= ammolim[groupnum])
				{
					 ammonum = 0;
				}
				else 
				{
					ammonum++;
				}
			}
			if (switch_state == 8)
			{
				chargenum ^= 1;
			}
			BTN_Down = 0; // push button is released so the button is no longer held down
			//LCDclr();
		}
	}
	if((timer0Overflows % 62) == 0) // push button polling on the release % 16
	{
		if(is_clear(PIND, Switch2)) // if the push button is held down connecting pin B2 to ground
		{
			switch_state--;
			LCDclr();
		}
		if(is_clear(PIND, Switch3)) // if the push button is held down connecting pin B2 to ground
		{
			switch_state++;
			LCDclr();
		}
		readAccelData(accelCount);
		for (int i=0; i<3; i++)
		{
			accelG[i] = (float)accelCount[i]/((1<<12)/(2*SCALE));  // get actual g value, this depends on scale being set
			accelD[i] = asin(accelG[i])*57.2957795;
			accelM[i] = asin(accelG[i])*1018.59163579;
		}
		accel3Axis[0] =  atan(accelG[0]/(sqrt((accelG[1]*accelG[1])+(accelG[2]*accelG[2]))))*1018.59163579;
		accel3Axis[1] =  atan(accelG[1]/(sqrt((accelG[0]*accelG[0])+(accelG[2]*accelG[2]))))*1018.59163579;
		accel3Axis[2] =  atan(sqrt((accelG[0]*accelG[0])+(accelG[1]*accelG[1]))/accelG[2])*1018.59163579;
		
		update_Display();
	}
	if((timer0Overflows % 125) == 0) // push button polling on the release 32
	{
		system_Clock++;
		if(is_clear(PINA, PowerSwitch)) // if the push button is held down connecting pin B2 to ground
		{
			power_count++;
			if(power_count == 5)
			{
				powerDRIdown();
				power_count = 0;
			}
		}
		else
		{
			power_count = 0;
		}
	
	}
	if((timer0Overflows % updateSpeed) == 0) // push button polling on the release
	{
		if (i == 1023) d = -1;   // count down
		if (i == -1024) d = 1;   // count up
		i += d;
		update_Display();
	}
	timer0Overflows++;
}

void timer0_init(void) // Initialize timer 0
{
	TCCR0A |= (0<<COM0A1)|(0<<COM0A0)|(0<<COM0B1)|(0<<COM0B0)|(0<<WGM01)|(0<<WGM00);
	TCCR0B |= (0<<FOC0A)|(0<<FOC0B)|(0<<WGM02)|(1<<CS02)|(0<<CS01)|(0<<CS00); //1024
	TIMSK0 |= (0<<OCIE0B)|(0<<OCIE0A)|(1<<TOIE0);
	TIFR0 |=  (0<<OCF0B)|(0<<OCF0A)|(0<<TOV0);
	

	timer0Overflows = 0; // Start timer overflows at 0
}

//delay 1s
void delay1s(void)
{
	uint8_t i;
	for(i=0;i<100;i++)
	{
		_delay_ms(10);
	}
}
//demonstration of progress bar
void progress(void)
{
	LCDclr();
	CopyStringtoLCD(OnScreen, 4, 0);
	/*
	LCDsendChar('D');
	delay1s();
	
	*/
	delay1s();
	for(uint8_t i=0;i<255;i++)
	{
		//_delay_ms(5);
		LCDGotoXY(4, 1);
		LCDprogressBar(i, 255, 8);
	}
	LCDclr();
}

void setupI2C(void)
{
	unsigned char c;

	// Read the WHO_AM_I register, this is a good test of communication
	c = readRegister(0x0D);  // Read WHO_AM_I register
	if (c == 0x2A) // WHO_AM_I should always be 0x2A
	{
		initMMA8452(SCALE, dataRate);  // init the accelerometer if communication is OK
	}
	else
	{
		while(1)
		{
			output_high(PORTD, GreenLED);
			_delay_ms(500);
			
			output_low(PORTD, GreenLED);
			_delay_ms(500);
		} // Loop forever if communication doesn't happen
	}
}

int trans(int elv, char * shltyp, int charge)
{
	int range;
	int i;
	int  j;


	/* generate pointers to correct shell type table ..
	initially ignoring illumination differences */

	fztime = 0;	/* assume 0 overwrite for illum rds */

	for(currentFT = firstFT; currentFT <= lastFT; currentFT++)
	{
		if(!(strcmp(shltyp,currentFT->shelltype)))
		{
			if(charge == 0)
			{
				first = currentFT->firstch0;

				/* check if charge 0 is a valid value..is except for illums normally */
				if(first == (const struct lookup *)0)
				{
					charge = 1;
					goto chg1;
				}
				/* above being true means only charge 1 valid */
				last = currentFT->lastch0;
				break;
			}
			chg1:
			if(charge == 1)
			{ /* would be else except for invalid charge 0 above */
				first = currentFT->firstch1;
				last = currentFT->lastch1;
				break;
			}
		}
	}

	gblcharge = charge;	/* usually the same as in but can override above */
	/* translate from elevation to range */

	if(pgm_read_word(&(first->mils)) < elv) return(-1);	/* too high */
	if(pgm_read_word(&(last->mils)) > elv)  return(-2);	/* too low */

	for(current = first; current < last ; current++) { /* here elv in bounds, get range in meters*/
		if(pgm_read_word(&(current->mils)) >= elv && pgm_read_word(&((current + 1)->mils)) < elv) {

			if((pgm_read_word(&(current->mils))  - elv) < (elv - pgm_read_word(&((current + 1)->mils))) ) { range = pgm_read_word(&(current->meters));
				i = current - first;

			}
			else {range = pgm_read_word(&((current + 1)->meters));
				i = current + 1 - first;

			}
			if(pgm_read_word(&(currentFT->fuzetimes))) fztime = (uint8_t*)((pgm_read_word(&(currentFT->fuzetimes))) + i); //fztime =*
			return(range);
		}
	}
	/**** here the elevation is exactly the value of the last elevation otherwise done above **/
	i = last - first;

	if(pgm_read_word(&(currentFT->fuzetimes))) fztime = (uint8_t*)((pgm_read_word(&(currentFT->fuzetimes))) + i); //fztime =*
	return(pgm_read_word(&(last->meters)));
}

void powerDRIdown(void)
{
	LCDclr();//clears LCD
	CopyStringtoLCD(PowerString, 0, 0);
	_delay_ms(2000);
	while(1)
	{
		output_low(PORTA, PowerOn);	
	}
}
	
int main(void)
{

	set_input(DDRA, Switch1);
	set_input(DDRD, Switch2);
	set_input(DDRD, Switch3);
	set_output(DDRD, GreenLED);
	set_output(DDRA, PowerOn);
	set_input(DDRA, PowerSwitch);
	
	output_high(PORTA, PowerOn);

	LCDinit();//init LCD bit, dual line, cursor right
	LCDclr();//clears LCD
	progress();
	delay1s();
	setupI2C();
	MMA8452Active();
	timer0_init();
	sei();
	
	while(1)//loop demos
	{
		
	}
}