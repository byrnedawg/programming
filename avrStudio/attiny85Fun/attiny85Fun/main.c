/*
 * attiny85Fun.c
 *
 * Created: 1/31/2017 8:15:51 PM
 * Author : Gregory Byrne
 */ 

/*
 * M19A1.c
 *
 * Created: 8/13/2015 8:32:47 AM
 *  Author: gregory.byrne
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdbool.h>

//												   ______
//	(PCINT5/~RESET/ADC0/dW)			Reset	--PB5 |		 |VCC--	5V
//	(PCINT3/XTAL1/CLKI/~OC1B/ADC3)	Batt	--PB3 |		 |PB2--	Switch						(SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
//	(PCINT4/XTAL2/CLKO/OC1B/ADC2) Latch On 	--PB4 |		 |PB1--	10K -- N-Chann Mosfett Gate	(MISO/DO/AIN1/OC0B/OC1A/PCINT1)
//											--GND |______|PB0--	Aux PWM						(MOSI/DI/SDA/AIN0/OC0A/~OC1A/AREF/PCINT0)
//


//Port Define Helpers
#define output_low(port,pin) port &= ~(1<<pin)		// output_low(PORTB, PB2);
#define output_high(port,pin) port |= (1<<pin)		// output_high(PORTB, PB2);
#define output_toggle(port,pin) port ^= (1<<pin)	// output_toggle(PORTB, PB2);
#define set_input(portdir,pin) portdir &= ~(1<<pin)		//set_input(DDRB, PB2);
#define set_output(portdir,pin) portdir |= (1<<pin)		//set_output(DDRB, PB2);
#define is_set(portpin,pin) (portpin & (1<<pin)) == (1<<pin)          //(for input) checks if the ... if(is_set(PINB,PB0)) input is high (Logic 1)
#define is_clear(portpin,pin) (portpin & (1<<pin)) == (0<<pin)		//(for input) checks if the ... if(is_clear(PINB,PB0)) input is low (Logic 0)


#define sbi(x,y) x |= _BV(y) //set bit (original sbi)
#define cbi(x,y) x &= ~(_BV(y)) //(original)cbi clear bit //reverse cause of 3904
#define tbi(x,y) x ^= _BV(y) //toggle bit
#define is_high(x,y) (x & _BV(y)) == _BV(y) //(for input) checks if the // input is high (Logic 1) i.e. if(is_high(PIND,PD0))
#define RESET &=~
#define SET |=
#define TOGGLE ^=
#define PWM_200Hz 0x27   //timer 1 OCR1C = PWM_200Hz;
#define PWM_30Hz 0xFC	//timer 1 OCR1C = PWM_30Hz;

#define Pot PB0
#define Divider PB1
#define Switch1 PB2
#define YellowLED PB3
#define GreenLED PB4

volatile uint8_t switch_state;	// Light Function state
volatile uint8_t last_switch_state; //Stores the last switch state for light wake up
volatile uint8_t Led_Strip_Brightness; // red pwm duty cycle
volatile uint8_t Power_Latch; // blue pwm duty cycle
volatile uint8_t Aux_PWM; // green pwm duty cycle
volatile uint8_t Light_On_Threshold; // Light on variable for analog digital converter
volatile uint16_t Sensor_Sample_Rate; // Controls the frequency the read ADC is called for the LDR
volatile uint16_t timer0Overflows; // Stores the value for timer polling timer 0
volatile uint16_t timer1Overflows; // Stores the value for timer polling timer 1
volatile uint16_t LDR_Value; // Value the ADC reads on the LDR pin PB3
volatile uint16_t Auto_Off_Seconds;
volatile uint16_t Auto_Off_Timer;
volatile uint16_t Auto_Off_Sample_Rate;
volatile bool Light_on; // true if LDR_Value > Light_On_Threshold
volatile bool BTN_Down; // true if the push button is being help down

ISR(ANA_COMP_vect)
{
	if(ACSR & (1 << ACO))
	{
		output_high(PORTB, YellowLED);
	}
	else
	{
		output_low(PORTB, YellowLED);
	}
}

ISR(ADC_vect) // Interrupt for ADC Converter
{
	uint8_t temp; // raw a2d variable
	temp = ADCH ; // ADCH is the high portion of the a2d
	LDR_Value = (56 * 255 / temp); // 56/ 1.1v = 255/5v (5v/wanted voltage level = 255/ 8 bit quantized voltage

	//if(LDR_Value > Light_On_Threshold) //132/2.6V Low Battery Voltage divider R1 324Ohms R2 200
	if(LDR_Value > Light_On_Threshold) //250
	{
		Light_on = true; // light should power on cause the LDR is reading it is dark
	}
	else
	{
		Light_on = false; // LDR senses light
		if(switch_state != 0) // If light is on
		{
			if(switch_state == 9) // If light is on but in the off mode
			{
				last_switch_state = 1; // store switch state as red so when it is dark again the unit powers on with a light and not in the off mode
			}
			else
			{
				last_switch_state = switch_state; // store the previous light state so when it is dark again the correct mode powers up
			}
		}
		switch_state = 0; // set to power off state because of light detected
	}

}

ISR(TIMER0_OVF_vect) // timer 0 overflow interrupt
{
	timer0Overflows++;	// 225 1 min // 1125 5 min // 2250 10min // 15 4seconds // keep track of timer 0 overflows for timer interrupt polling
	if((timer0Overflows % 5) == 0) //15 4 seconds
	{
		if(switch_state == 0 && Light_on == true) // if the light is off and its supposed to be on
		{
			switch_state = last_switch_state; // restore previous powered on switch state
			last_switch_state = 1; // set last switch state to 1
		}
		if(switch_state == 1)	//Red
		{
			Led_Strip_Brightness = 51; // **** NOTE: Red and green should be switched and for there timer 0 is 100% and 255 is 0%
			Aux_PWM = 0;
			Power_Latch = 255;  // **** NOTE: for blue 0 is 0% and 255 is 100%
		}
		else if(switch_state == 2)	//Orange
		{
			Led_Strip_Brightness = 102; //245
			Aux_PWM = 0; //165
			Power_Latch = 255;
		}
		else if(switch_state == 3)	//Yellow
		{
			Led_Strip_Brightness = 153; //40 is less brighter than 0
			Aux_PWM = 0; //40 is less brighter than 0
			Power_Latch = 255;
		}
		
		else if(switch_state == 4)	//Green
		{
			Led_Strip_Brightness = 204;
			Aux_PWM = 0;
			Power_Latch = 255;
		}
		else if(switch_state == 5)	//Blue
		{
			Led_Strip_Brightness = 255;
			Aux_PWM = 0;
			Power_Latch = 255;
		}
		else if(switch_state == 6)	//Blue
		{
			Led_Strip_Brightness = 0;
			Aux_PWM = 0;
			Power_Latch = 0;
		}
		else if(switch_state == 0)	//Light detected OFF
		{
			Led_Strip_Brightness = 0;
			Aux_PWM = 0;
			Power_Latch = 0;
			switch_state = 0;
		}
		else
		{
			
			switch_state = 0; // set switch state to 0 if the state is above 9
			
		}
		
		OCR0B = (255 - Led_Strip_Brightness);  // PWM Duty cycle value
		OCR0A = (255 - Aux_PWM); // PWM Duty cycle value switched green and red
		OCR1B = Power_Latch; // PWM Duty cycle value
	}

	if((timer0Overflows % Sensor_Sample_Rate) == 0) //  Sensor_Sample_Rate = %500
	{
		ADCSRA|=(1<<ADSC); // setting ADSC to 1 starts A2D Conversion
		if(Light_on == false) // return from A2D interrupt check if the light should be on
		{
			switch_state = 0; // if light should be off set to state 0 or the off state
		}
		
	}
	
	if((timer0Overflows % Auto_Off_Sample_Rate) == 0) //  Sensor_Sample_Rate = %500
	{	
		if(Auto_Off_Timer >= Auto_Off_Seconds) // Value in Seconds
		{
			switch_state = 6; // if light should be off set to state 0 or the off state
			Auto_Off_Timer = 0;
		}
		else
		{
			Auto_Off_Timer++;
		}
		
	}

	
}
ISR(TIMER0_COMPA_vect) // timer 0 compare A interrupt
{
	
}

ISR(TIMER0_COMPB_vect) // timer 0 compare B interrupt
{

}

ISR(TIMER1_OVF_vect) // timer 1 overflow interrupt
{
	timer1Overflows++; //keep track of timer 1 overflows for timer interrupt polling
	
	if((timer1Overflows % 20) == 0) // push button polling 195 1 sec /975 5 sec/ 11700 1 min/
	{
		if(is_clear(PINB, 2)) // if the push button is held down connecting pin B2 to ground
		{
			BTN_Down = true; // button down is true
			Auto_Off_Timer = 0;
		}
	}
	if((timer1Overflows % 25) == 0) // push button polling on the release
	{

		if(is_set(PINB, 2) && BTN_Down) // push button release to set pin B2 to high again after being held low
		{
			//sbi(PORTB,PB3);
			switch_state++; // increment mode states by 1 i.e. red -> yellow, green - > blue
			last_switch_state = switch_state; // update the last switch state to hold the value of switch state in case of a light power down
			BTN_Down = false; // push button is released so the button is no longer held down
			Auto_Off_Timer = 0;
		}
	}
	
}



ISR(TIMER1_COMPA_vect) // timer 1 compare A interrupt
{

}

ISR(TIMER1_COMPB_vect) // timer 1 compare B interrupt
{

}

ISR(PCINT0_vect) // Pin Change interrupt service routine
{
	//sleep_disable(); // If pins change state wake up
	if(is_set(PINB, Switch1))
	{
		output_high(PORTB, GreenLED);
	}
	else
	{
		output_low(PORTB, GreenLED);
	}
	//TCNT0 = 0x00; // reset timer 0
	//TCNT1 = 0x00; // reset timer 1
	//timer0Overflows = 0; // reset timer o overflows
	//timer1Overflows = 0; // dont reset timer 1 overflows keep running forever
}

void PortInit(void) // initialize port B inputs and outputs
{
	//												   ______
	//	(PCINT5/~RESET/ADC0/dW)			Reset	--PB5 |		 |VCC--	5V
	//	(PCINT3/XTAL1/CLKI/~OC1B/ADC3)	YLW LED --PB3 |		 |PB2--	Switch1	(SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
	//	(PCINT4/XTAL2/CLKO/OC1B/ADC2)	GRN LED	--PB4 |		 |PB1--	Divider	(MISO/DO/AIN1/OC0B/OC1A/PCINT1)
	//											--GND |______|PB0--	POT	(MOSI/DI/SDA/AIN0/OC0A/~OC1A/AREF/PCINT0)
	//

	//PORTB |= (1<<PB4)|(1<<PB3)|(1<<PB2)|(1<<PB1)|(1<<PB0); // set all pins we are using high
	//DDRB |= (1<<DDB4)|(1<<DDB3)|(1<<DDB2)|(1<<DDB1)|(0<<DDB0); // set PB4, PB1 and PB0 as outputs and PB3 and PB2 as inputs
	//PORTB |= (0<<PB4)|(0<<PB3)|(0<<PB2)|(0<<PB1); // set all outputs to 0
	// make sure pull-up resistors are turned off
	PORTB = 0x00;
	DDRB = 0x00;
	set_input(DDRB, Pot);
	set_input(DDRB, Divider);
	set_input(DDRB, Switch1);
	set_output(DDRB, YellowLED);
	set_output(DDRB, GreenLED);
	
	output_high(PORTB, YellowLED);
	output_high(PORTB, GreenLED);
	
}

void SystemInit(void) // Power on variable initialization
{
	//BTN_Down = false; // button is not pushed down
	PCMSK |= (1<<PCINT2); // pin change mask: listen to portb bit 0 PB0
	GIMSK |= (1<<PCIE); // enable PCINT interrupt
	MCUCR |= (1<<BODS)|(0<<PUD)|(1<<SE)|(0<<SM1)|(1<<SM0)|(0<<BODSE)|(1<<ISC01)|(0<<ISC00); // disable BOD during sleep, Sleep Enable, ADC Noise Reduction, The falling edge of INT0 generates an interrupt request
	sei(); // enable all interrupts
	//timer0Overflows = 0; // time0roverflows set to 0
	//timer1Overflows = 0; // time1roverflows set to 0
}

void timer0_init(void) // Initialize timer 0
{
	// NOTE: Assumes DIV8 fuse set for 1 MHz operation
	// 8-bit counter means overflow interrupt every 61 Hz to turn on the LED
	// Variable Compare_A interrupt in between OVF to turn off the LED
	TCCR0A |= (1<<COM0A1)|(1<<COM0A0)|(1<<COM0B1)|(1<<COM0B0)|(1<<WGM01)|(1<<WGM00); // Set OC0A/OC0B on Compare Match, clear OC0A/OC0B at BOTTOM (inverting mode) ,Fast PWM Mode
	TCCR0B |= (0<<FOC0A)|(0<<FOC0B)|(0<<WGM02)|(0<<CS02)|(1<<CS01)|(0<<CS00); //0x02;	pre-scale= CK/8
	OCR0A = 0x00; // Governs Duty Cycle (192 ~75% of 256) for inverting mode OCR0A = (255-duty-cycle)
	OCR0B = 0x00; // Governs Duty Cycle (192 ~75% of 256) for inverting mode OCR0B = (255-duty-cycle)
	TCNT0 = 0x00; // Start Timer
	timer0Overflows = 0; // Start timer overflows at 0
}

void timer1_init(void) // Initialize timer 1
{
	TCCR1 |= (0<<CTC1)|(0<<PWM1A)|(0<<COM1A1)|(0<<COM1A0)|(1<<CS13)|(0<<CS12)|(0<<CS11)|(0<<CS10); // PWM1A Disabled, Timer/Counter Comparator A disconnected from output pin OC1A, prescale= CK/128 right now was CK/8 1000b ~ 1100b
	GTCCR |= (0<<TSM)|(1<<PWM1B)|(1<<COM1B1)|(0<<COM1B0)|(0<<FOC1B)|(0<<FOC1A)|(0<<PSR1)|(0<<PSR0); //PWM1B Enabled, OC1B cleared on compare match. Set when TCNT1 = $00
	//OCR1A = 0x00; // Disabled Governs Duty Cycle (128 ~50% of 256)
	OCR1B = 0x00; // Governs Duty Cycle (192 ~75% of 256)
	OCR1C = PWM_200Hz;	// OCR1C = Governs PWM Frequency :: (1Mhz/prescaler)/(OCR1C+1)= PWM Frequency :: 1M/128/39+1 = 200Hz	0x27 1M/128/252+1 = 33Hz	0xFC
	TIMSK |= (0<<OCIE1A)|(1<<OCIE1B)|(0<<OCIE0A)|(0<<OCIE0B)|(1<<TOIE1)|(1<<TOIE0);	// Compare A and OVF Interrupt enabled
	TCNT1 = 0x00; // Start Timer
	timer1Overflows = 0; // Start timer overflows at 0
}

void Analog_Comparator_init(void) // Initialize Analog Comparator
{
	ADCSRB |= (0<<BIN)|(0<<ACME)|(0<<IPR)|(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0); //Analog Comparator Multiplexer Enable
	ACSR |= (0<<ACD)|(0<<ACBG)|(0<<ACO)|(0<<ACI)|(1<<ACIE)|(0<<ACIS1)|(0<<ACIS0); // Analog Comparator Control Register
	DIDR0 |= (0<<ADC0D)|(0<<ADC2D)|(0<<ADC3D)|(0<<ADC1D)|(0<<AIN1D)|(0<<AIN0D); // Digital input Disable Register
}

void ADC_init(void) // Initialize Analog to Digital Converter
{
	ADMUX |= (0<<REFS1)|(0<<REFS0)|(1<<ADLAR)|(0<<REFS2)|(0<<MUX3)|(0<<MUX2)|(1<<MUX1)|(1<<MUX0); //ADC Multiplexer Selection Register... (ADC3(PB3) Selected ADC Input)
	ADCSRA |= (1<<ADEN)|(1<<ADSC)|(0<<ADATE)|(0<<ADIF)|(1<<ADIE)|(0<<ADPS2)|(0<<ADPS1)|(0<<ADPS0); // ADC Control Register A
	ADCSRB |= (0<<BIN)|(0<<ACME)|(0<<IPR)|(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0); // ADC Control Register B
	DIDR0 |= (0<<ADC0D)|(0<<ADC2D)|(0<<ADC3D)|(0<<ADC1D)|(0<<AIN1D)|(0<<AIN0D); // Digital input Disable Register
}

int main(void)
{
	//timer0_init(); // timer 0 initialize
	//timer1_init(); // timer 1 initialize
	//ADC_init(); // analog to digital converter initialize
	Analog_Comparator_init();
	PortInit(); // inputs and outputs initialize
	SystemInit(); // system variables and state initialize

	while(1)
	{
		sleep_enable(); // sleep until pin interrupt
	}
	
	return 0;
}

