#define F_CPU 8000000UL
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

//static volatile bool ledState = false;
int main(void)
{

	DDRB |= (1 << PB5); //set PB0 as output
	
	
    while (1)
    {	
		PORTB ^= (1<<PB5);
		_delay_ms(1000);
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

ISR (TIMER0_COMPA_vect)  // timer0 overflow interrupt
{
	static int count = 0;
	
	if(++count < 50)
		return;
	
	count = 0;
	PORTD ^= (1 << PD6);
    //event to be exicuted every 4s here
}