#include <avr/io.h>
#define F_CPU 10000000  // system clock is 20 MHz
#include <util/delay.h>  // uses F_CPU to achieve us and ms delays
 
//Functions that control the car. 
//Fnt = front wheels, Bak = back wheels
//fwd = forward, rev = reverse
void fwdFnt()
{
	PORTB |= 0b00100000;
	PORTD |= 0b10000000;
}

void fwdFnt(unsigned char pwm)
{
	OCR0A = OCR0B = pwm;
	PORTB |= 0b00100000;
	PORTD |= 0b10000000;
}

void revFnt()
{
	PORTB |= 0b00010001;
	PORTD |= 0b00000000;
}

void revFnt(unsigned char pwm)
{	
	OCR0A = OCR0B = pwm;
	PORTB |= 0b00010001;
	PORTD |= 0b00000000;
}

void fwdBak()
{
	PORTB |= 0b00000010;
	PORTD |= 0b00010000;
} 

void fwdBak(unsigned char pwm)
{
	OCR2A = OCR2B = pwm;
	PORTB |= 0b00000010;
	PORTD |= 0b00010000;
} 

 void revBak()
{
	PORTB |= 0b00000100;
	PORTD |= 0b00000100;
}

 void revBak(unsigned char pwm)
{
	OCR2A = OCR2B = pwm;
	PORTB |= 0b00000100;
	PORTD |= 0b00000100;
}

void revAll(unsigned char pwm)
{
	OCR0A = OCR0B = OCR2A = OCR2B = pwm;
	revFnt();
	revBak();
}

void fwdAll(unsigned char pwm)
{
	OCR0A = OCR0B = OCR2A = OCR2B = pwm;
	fwdFnt();
	fwdBak();
}

void revAll()
{
	revFnt();
	revBak();
}

void fwdAll()
{
	fwdFnt();
	fwdBak();
}
 
// Motor Initialization routine -- this function must be called
//  before you use any of the above functions
void motors_init()
{
    // configure for inverted PWM output on motor control pins:
    //  set OCxx on compare match, clear on timer overflow
    //  Timer0 and Timer2 count up from 0 to 255
    TCCR0A = TCCR2A = 0xF3;
 
    //prescaler /1028
    TCCR0B = TCCR2B = 0x05; //start the clock
 
    // start clocks
    OCR0A = OCR0B = OCR2A = OCR2B = 0;
 
	// set pins as output
	DDRD |= (1 << PD7) | (1 << PD4) | (1 << PD2) | (1 << PD3) | (1 << PD6) | (1 << PD0) | (1 << PD5);
	DDRB |= (1<< PB0) |(1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);
}


//I took this code from online. 
// delay for time_ms milliseconds by looping
//  time_ms is a two-byte value that can range from 0 - 65535
//  a value of 65535 (0xFF) produces an infinite delay
void delay_ms(unsigned int time_ms)
{
    // _delay_ms() comes from <util/delay.h> and can only
    //  delay for a max of around 13 ms when the system
    //  clock is 20 MHz, so we define our own longer delay
    //  routine based on _delay_ms()
 
    unsigned int i;
 
    for (i = 0; i < time_ms; i++)
        _delay_ms(1);       
}
 
int main()
{
    motors_init();
	
	//fwdFnt();
	//fwdFnt(100); //the smaller the nb the faster the speed. 200 seems to be too weak
	fwdFnt(100);
	revBak(100);
   
    while (1)
	{
		
	}
        
    return 0;
}
