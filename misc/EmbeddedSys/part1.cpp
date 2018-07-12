#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>


	//DDRB |= (1<<0); //sets output on both sides of the Atmega
	//DDRD |= (1<<0);

	//DDRB |= (1 << PB4) | (1 << PB5);
	//PB5 -> 4A (top right wheels), PB4 ->3A (top right), PB0 -> 1A (top left), PB7 -> 2A(top chip left)
	//PD4 -> 4A (bottom right wheel), PB3 ->3A (bottom right wheel), PD2 -> 1A (bottom left wheel), PD3 -> 2A(bottom left wheel)
	//top chip


void M1_forward(unsigned char pwm)
{
  OCR0A = 0;
 // OCR0B = pwm;
	OCR2A = pwm;
}
 
void M1_reverse(unsigned char pwm)
{
  OCR0B = 0;
  OCR0A = pwm;
}
 
void M2_forward(unsigned char pwm)
{
  OCR2A = 0;
  OCR2B = pwm;
}
 
void M2_reverse(unsigned char pwm)
{
  OCR2B = 0;
  OCR2A = pwm;
}

void motors_init()
{
	// configure for inverted PWM output on motor control pins:
	//  set OCxx on compare match, clear on timer overflow
	//  Timer0 and Timer2 count up from 0 to 255
	TCCR0A = TCCR2A = 0xF3;
 
	// use the system clock/8 (=2.5 MHz) as the timer clock
	TCCR0B = TCCR2B = 0x01;
 
	// initialize all PWMs to 0% duty cycle (braking)
	OCR0A = OCR0B = OCR2A = OCR2B = 0;
 
	// set PWM pins as digital outputs (the PWM signals will not
	// appear on the lines if they are digital inputs)
	DDRD |= (1 << PD5) | (1 << PD6); //top hbridge
	DDRB |= (1 << PB2) | (1 << PB1); //bottom hbridge
}


int main()
{
    motors_init();
	DDRD |= (1 << PD2) | (1 << PD3);
	
	
    M1_forward(255);  // motor 1 forward at half speed
    M2_reverse(255);  // motor 2 reverse at 10% speed
 
    _delay_ms(1000);  // delay for 2s while motors run
 
    M1_reverse(255);  // motor 1 reverse at 25% speed
    M2_forward(255);  // motor 2 stop/brake
 
    // loop here forever to keep the program counter from
    //  running off the end of our program
    while (1)
        ;
 
    return 0;
}
	