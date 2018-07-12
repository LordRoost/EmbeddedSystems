#define CPU_16MHz 0x00
#define F_CPU 16000000L

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <SharpIR.h>

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

/*the first 3 defines indicate the Teensy frequency, 
  which will allow the clock functions to scale */


int main(void)
{
	CPU_PRESCALE(CPU_16MHz);
	int ch, dis; 
	SharpIR sharp(28, 25, 93, 1080)	
	
	//set multiplexer to read. REFS0 is the reference voltage.
	ADMUX |= (1<<REFS0); 
	//sets up Teensy frequency to adjust to hardware
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);  
	ADMUX |= ch;   
	
	while(1)
	{
		ADCSRA|=(1<<ADSC); //gets ready to read
		while(!(ADCSRA && (1<<ADIF))); //waits until reading is done
		dis = sharp.distance();
		itoa(ADC, buffer, 5);
		ADCSRA |= (1<<ADIF); //resets interrrupt flag
	}
}