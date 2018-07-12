#define F_CPU 1000000L

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

//what should be compelted: I/O console input and reply. Speed of one wheel. 
//TODO: debugging. update it to work on on more than one wheel. And go in reverse
//if char does not work, and the input is uin8_t, then it means that the buffer must be created to an array of chars. 

/*
void initSlave()
{
	DDRB |= (1<<PB4);                 //set MISO as output
	SPCR = (1<<SPE)|(1<<SPIE);        //enable SPI and enable SPI interrupt
}*/

void initWheels()
{
	/*
	Bellow we declare pins for to be used for motors.
	Pin PD6 is the enabler that controls the speed of all the motors 
	*/
	DDRD |= (1 << PD4) | (1 << PD6) | (1 << PD7); 
	DDRC |= (1 << PC0) | (1 << PC1);
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2)| (1 << PB6) | (1 << PB7); //A inputs (directions of wheels) [PD2: Back] [PD3: front]

    TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1<< WGM00);
	TCCR0B |= (1 << CS00);

	OCR0A = 0;

	//PORTB |= (0 << PB5) | (1 << PB4);
}

//Directional control methods
void backFwd()
{
	//BACK LEFT wheel
	PORTB |= (1<< PB6);
	PORTB &= ~(1<< PB7);

	//BACK RIGHT wheel
	PORTD |= (1<< PD4);
	PORTB &= ~(1 << PB1);
}

void backRev()
{
	//BACK LEFT wheel
	PORTB &= ~(1<< PB6);
	PORTB |= (1<< PB7);

	//BACK RIGHT wheel
	PORTD &= ~(1<< PD4);
	PORTB |= (1 << PB1);
}

void frontFwd()
{
	//FRONT LEFT wheel
	PORTD |= (1 << PD7);
	PORTB &= ~(1 << PB0);

	//FRONT RIGHT wheel
	PORTC |= (1 << PC1);
	PORTC &= ~(1 <<PC0);
}

void frontRev()
{
	//FRONT LEFT wheel
	PORTD &= ~(1 << PD7);
	PORTB |= (1 << PB0);

	//FRONT RIGHT wheel
	PORTC &= ~(1 << PC1);
	PORTC |= (1 <<PC0);
}

void rightFwd()
{
	//BACK RIGHT wheel
	PORTD |= (1<< PD4);
	PORTB &= ~(1 << PB1);
	
	//FRONT RIGHT wheel
	PORTC |= (1 << PC1);
	PORTC &= ~(1 <<PC0);
}

void rightRev()
{
	//FRONT RIGHT wheel
	PORTD &= ~(1<< PD4);
	PORTB |= (1 << PB1);
	
	//BACK RIGHT wheel
	PORTC &= ~(1 << PC1);
	PORTC |= (1 <<PC0);
}

void leftFwd()
{
	//BACK LEFT wheel
	PORTB |= (1<< PB6);
	PORTB &= ~(1<< PB7);
	
	//FRONT LEFT wheel
	PORTD |= (1 << PD7);
	PORTB &= ~(1 << PB0);
}

void leftRev()
{
	//BACK LEFT wheel
	PORTB &= ~(1<< PB6);
	PORTB |= (1<< PB7);
	
	//FRONT LEFT wheel
	PORTD &= ~(1 << PD7);
	PORTB |= (1 << PB0);	
}

void goForward()
{	
	frontFwd();
	backFwd();
}

void goBackward()
{
	frontRev();
	backRev();
}


/*
Set all LEFT wheels to go backwards and all RIGHT wheels to go forwards
*/
void turnLeft()
{
	leftRev();
	rightFwd();
}

/*
Set all RIGHT wheels to go backwards and all LEFT wheels to go forwards
*/
void turnRight()
{
	leftFwd();
	rightRev();
}

//Turn 90 degree RIGHT
void turnRight_90()
{
    turnRight();
    _delay_ms(500);
}

//Turn 90 degree LEFT
void turnLeft_90()
{
    turnLeft();
    _delay_ms(500);
}



int main(void){
	
	initWheels();
	//initSlave();
	
	OCR0A = 255;

    	while(1)
   	    {
			//goForward();
			turnLeft();
    	}
    	return 0;   
}

/*
ISR(SPI_STC_vect) //Interrupt for when serial transfer is finished
{ 	
	switch(SPDR)
	{
		case 1 : //forwards
			goForward();
			break;     
		case 2 : //backwards
			goBackward();
			break;
		case 3 : //left
			turnLeft();
			break;       
		case 4 : //right
			turnRight();
			break;
	}
		
}
*/