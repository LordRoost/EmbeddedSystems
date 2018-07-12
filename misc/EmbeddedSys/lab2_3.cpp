#define F_CPU 8000000UL
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

//static volatile bool ledState = false;
int main(void)
{

	DDRD |= (0 << PD2); //set PD2 as input
	DDRB |= (1 << PB0); //set PB0 as output
	
	PORTD |= (1 << PORTD2);    // turn On the Pull-up
    // PD2 is now an input with pull-up enabled

    EICRA |= (1 << ISC01);    // set INT0 to trigger on falling edge
	
	EIMSK |= (1 << INT0);     // Turns on INT0
	
	//Enable interrupts
    sei();         

    while (1)
    {	
		
    }
}


ISR(INT0_vect)
{
	//cli();
	/*if(ledState == false)
	{
		PORTB |= (1 << PB0);
		ledState = true;
	}
	else{
		PORTB &= ~(1 << PB0);
		ledState = false;
	}*/
	//PORTB ^= (1 << PB0); //toggle LED
	PORTB &= ~(1 << PB0); //toggle LED
	
	//sei();	
}