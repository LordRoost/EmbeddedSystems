#include <avr/io.h> 
#include <avr/interrupt.h>


int main(void)
{
	
	DDRD |= (1 << PD6); //set PD6 as output
	
    TCCR1A = 0;
	TCCR1B = 0;
	
    // set up timer with prescaler = 256 and CTC mode
	TCCR1B |= (1 << WGM12)|(1 << CS12);
  
    // initialize counter
    TCNT1 = 0;
  
    // initialize compare value
    //OCR1A = 15624; ~1 sec
	OCR1A = 20000;

    TIMSK1 = (1 << OCIE1A);   //enable interrupt for compare

    sei();         //enable global interrupts


    while (1)
    {
        //main loop
    }
}

ISR (TIMER1_COMPA_vect)  // timer1 compare interrupt
{
	PORTD ^= (1 << PD6); //event to be executed every 5s here
}