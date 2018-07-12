#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

int main()
{

	DDRD |= (1 << PD6);
	
	TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1<< WGM00);
	
	TCCR0B |= (1 << CS00);
	
	OCR0A = 255;
	
	while(1)
	{
		
		PORTD |= (1 << PD6);
		_delay_ms(1000); // Wait 1 sec
		
	
		OCR0A = OCR0A / 2;
		_delay_ms(1000);
		
		OCR0A = OCR0A / 2;
		_delay_ms(1000);
		
		OCR0A = OCR0A / 2;
		_delay_ms(1000);
		
		OCR0A = 255;
	}
}