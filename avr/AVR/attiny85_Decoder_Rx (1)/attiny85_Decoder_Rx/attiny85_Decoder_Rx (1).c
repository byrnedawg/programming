/*
 * attiny85_Decoder_Rx.c
 *
 * Created: 12/28/2015 6:58:53 AM
 *  Author: ByrneDawg
 */ 


#include <avr/io.h>

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <util/atomic.h>
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

#define RxInput PB0
#define Light_Output PB4

volatile uint16_t timer0Overflows = 0; // Stores the value for timer polling timer 0
volatile uint16_t timer1Overflows = 0; // Stores the value for timer polling timer 0
volatile bool Light_on = true; // true if LDR_Value > Light_On_Threshold
volatile bool BTN_Down = false; // true if the push button is being help down
volatile uint8_t Auto_Off_Timer = 0;

ISR(ANA_COMP_vect)
{

}

ISR(ADC_vect) // Interrupt for ADC Converter
{
	
}

ISR(TIMER0_OVF_vect) // timer 0 overflow interrupt
{
	timer0Overflows++;	// 225 1 min // 1125 5 min // 2250 10min // 15 4seconds // keep track of timer 0 overflows for timer interrupt polling

	
}
ISR(TIMER0_COMPA_vect) // timer 0 compare A interrupt
{
	
}

ISR(TIMER0_COMPB_vect) // timer 0 compare B interrupt
{

}

ISR(TIMER1_OVF_vect) // timer 1 overflow interrupt
{
	/*
	timer1Overflows++; //keep track of timer 1 overflows for timer interrupt polling
	
	if((timer1Overflows % 20) == 0) // push button polling 195 1 sec /975 5 sec/ 11700 1 min/
	{
		if(is_set(PORTB, RxInput)) // if the push button is held down connecting pin B2 to ground
		{
			//BTN_Down = true; // button down is true
			Auto_Off_Timer = 0;
			output_high(PORTB, Light_Output);
		}
		else
		{
			Auto_Off_Timer++;
			if(Auto_Off_Timer > 5)
			{
				output_low(PORTB, Light_Output);
			}
			else
			{
				output_high(PORTB, Light_Output);
			}
			
			
		}
	}
	if((timer1Overflows % 30) == 0) // push button polling on the release
	{

		if(is_set(PORTB, RxInput)) // push button release to set pin B2 to high again after being held low
		{
			Auto_Off_Timer = 0;
			output_high(PORTB, Light_Output);
		}
		else
		{
			Auto_Off_Timer++;
			if(Auto_Off_Timer > 5)
			{
				output_low(PORTB, Light_Output);
			}
			else
			{
				output_high(PORTB, Light_Output);
			}
		}
	}
	*/
}



ISR(TIMER1_COMPA_vect) // timer 1 compare A interrupt
{

}

ISR(TIMER1_COMPB_vect) // timer 1 compare B interrupt
{

}

ISR(PCINT0_vect) // Pin Change interrupt service routine
{
	sleep_disable(); // If pins change state wake up
	TCNT0 = 0x00; // reset timer 0
	TCNT1 = 0x00; // reset timer 1
	timer0Overflows = 0; // reset timer o overflows
	timer1Overflows = 0; // 
}

void PinChangeInit(void)
{
	PCMSK |= (1<<PCINT0); // pin change mask: listen to portb bit 2 PB2
	GIMSK |= (1<<PCIE); // enable PCINT interrupt
	MCUCR |= (1<<BODS)|(0<<PUD)|(1<<SE)|(0<<SM1)|(1<<SM0)|(0<<BODSE)|(1<<ISC01)|(0<<ISC00); // disable BOD during sleep, Sleep Enable, ADC Noise Reduction, The falling edge of INT0 generates an interrupt request
}

void PortInit(void) // initialize port B inputs and outputs
{
	//												   ______
	//	(PCINT5/~RESET/ADC0/dW)			Reset	--PB5 |		 |VCC--	5V
	//	(PCINT3/XTAL1/CLKI/~OC1B/ADC3)	LDR		--PB3 |		 |PB2--	Switch	(SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
	//	(PCINT4/XTAL2/CLKO/OC1B/ADC2)	BLU LED	--PB4 |		 |PB1--	RED LED	(MISO/DO/AIN1/OC0B/OC1A/PCINT1)
	//											--GND |______|PB0--	GRN LED	(MOSI/DI/SDA/AIN0/OC0A/~OC1A/AREF/PCINT0)
	//

	set_input(DDRB, RxInput);
	set_output(DDRB, Light_Output);
	
	output_low(PORTB, Light_Output);
}

void SystemInit(void) // Power on variable initialization
{
	sei(); // enable all interrupts
	timer0Overflows = 0; // time0roverflows set to 0
	timer1Overflows = 0; // time1roverflows set to 0
}

void timer0_init(void) // Initialize timer 0
{
	// NOTE: Assumes DIV8 fuse set for 1 MHz operation
	// 8-bit counter means overflow interrupt every 61 Hz to turn on the LED
	// Variable Compare_A interrupt in between OVF to turn off the LED
	// 1000 0010
	// 1000 0011
	TCCR0A |= (0<<COM0A1)|(0<<COM0A0)|(0<<COM0B1)|(0<<COM0B0)|(0<<WGM01)|(0<<WGM00); // Set OC0A/OC0B on Compare Match, clear OC0A/OC0B at BOTTOM (inverting mode) ,Fast PWM Mode
	TCCR0B |= (1<<FOC0A)|(0<<FOC0B)|(0<<WGM02)|(0<<CS02)|(1<<CS01)|(1<<CS00); //0x02;	pre-scale= CK/8
	OCR0A = 0x7D; // Governs Duty Cycle (192 ~75% of 256) for inverting mode OCR0A = (255-duty-cycle)
	OCR0B = 0x00; // Governs Duty Cycle (192 ~75% of 256) for inverting mode OCR0B = (255-duty-cycle)
	TCNT0 = 0x00; // Start Timer
	timer0Overflows = 0; // Start timer overflows at 0
	
	
	// SREG_I_bit = 1;
	 //OCR0A = 125;
	 //TCCR0A = 0x82;
	 //TCCR0B |= 0x83;
	// OCIE0A_bit = 1;

	
}

void timer1_init(void) // Initialize timer 1
{
	// 1000 0110
	
	TCCR1 |= (1<<CTC1)|(0<<PWM1A)|(0<<COM1A1)|(0<<COM1A0)|(0<<CS13)|(1<<CS12)|(1<<CS11)|(0<<CS10); // PWM1A Disabled, Timer/Counter Comparator A disconnected from output pin OC1A, prescale= CK/128 right now was CK/8 1000b ~ 1100b
	GTCCR |= (0<<TSM)|(0<<PWM1B)|(0<<COM1B1)|(0<<COM1B0)|(0<<FOC1B)|(0<<FOC1A)|(0<<PSR1)|(0<<PSR0); //PWM1B Enabled, OC1B cleared on compare match. Set when TCNT1 = $00
	OCR1A = 0xFA; // Disabled Governs Duty Cycle (128 ~50% of 256)
	//OCR1B = 0x00; // Governs Duty Cycle (192 ~75% of 256)
	//OCR1C = PWM_200Hz;	// OCR1C = Governs PWM Frequency :: (1Mhz/prescaler)/(OCR1C+1)= PWM Frequency :: 1M/128/39+1 = 200Hz	0x27 1M/128/252+1 = 33Hz	0xFC
	TIMSK |= (1<<OCIE1A)|(0<<OCIE1B)|(1<<OCIE0A)|(0<<OCIE0B)|(1<<TOIE1)|(1<<TOIE0);	// Compare A and OVF Interrupt enabled
	TCNT1 = 0x00; // Start Timer
	timer1Overflows = 0; // Start timer overflows at 0
	
	
	//SREG_I_bit = 1;
	//OCR1A = 250;
	//TCCR1 = 0x86;
	//OCIE1A_bit = 1;
}

void Analog_Comparator_init(void) // Initialize Analog Comparator
{
	ADCSRB |= (0<<BIN)|(0<<ACME)|(0<<IPR)|(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0); //Analog Comparator Multiplexer Enable
	ACSR |= (0<<ACD)|(0<<ACBG)|(0<<ACO)|(0<<ACI)|(0<<ACIE)|(0<<ACIS1)|(0<<ACIS0); // Analog Comparator Control Register
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
	timer0_init(); // timer 0 initialize
	timer1_init(); // timer 1 initialize
	ADC_init(); // analog to digital converter initialize
	PortInit(); // inputs and outputs initialize
	PinChangeInit();
	SystemInit(); // system variables and state initialize
	output_high(PORTB,Light_Output);
	_delay_ms(1000);
	output_low(PORTB, Light_Output );
	_delay_ms(1000);

	while(1)
	{
		if(is_set(PINB, RxInput) && Light_on == true) // if the push button is held down connecting pin B2 to ground
		{
			//BTN_Down = true; // button down is true
			Auto_Off_Timer = 0;
			//output_high(PORTB, Light_Output);
			Light_on = true;
		}
		else if(is_set(PINB, RxInput) && Light_on == false)
		{
			_delay_ms(100);
			if(is_set(PINB, RxInput))
			{
				Auto_Off_Timer = 0;
				//output_high(PORTB, Light_Output);
				Light_on = true;
			}
			else
			{
				Light_on = false;
			}
		}
		else
		{
			Auto_Off_Timer++;
			if(Auto_Off_Timer > 5)
			{
				//output_low(PORTB, Light_Output);
				Light_on = false;
			}
			else
			{
				output_high(PORTB, Light_Output);
				Light_on = true;
			}
		}
		if(Light_on == true)
		{
			output_high(PORTB, Light_Output);
		}
		else
		{
			output_low(PORTB, Light_Output);
		}
		_delay_ms(100);

		//sleep_enable(); // sleep until pin interrupt
	}
}
