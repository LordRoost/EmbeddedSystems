//Code for Slave
#include <avr/io.h>
#include <stdlib.h>
#define F_CPU 1000000UL  
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUDRATE 4800
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

char buffer[5];                          
void USART_init(void);                   
void USART_send( unsigned char data);    
void USART_putstring(char* StringPtr);   


void USART_send( unsigned char data) //USART send to UDR0
{   
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void USART_putstring(char* StringPtr)  //send string as characters
{   
	while(*StringPtr != 0x00)
	{
	   USART_send(*StringPtr);
	   StringPtr++;
	}
}

int main(void)
{
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
	DDRB |= (1<<PB4);                 //set MISO as output
	SPCR = (1<<SPE)|(1<<SPIE);        //enable SPI and enable SPI interrupt
	TCNT0=0;                          //Set Initial Timer value
	OCR0A=0;                          //Place compare value to Output compare register
	TCCR0A|=(1<<COM0A1)|(1<<WGM01)|(1<<WGM00); //Set fast PWM mode
	TCCR0B|=(1<<CS01)|(1<<CS00);  //Set prescaler to 64 and start timer
	sei(); //enable global interrupts
	
	
	while (1)
	{
    //main loop
	}
}

ISR(SPI_STC_vect) //Interrupt for when serial transfer is finished
{ 	
	USART_putstring("IR Sensor Reading: ");
	itoa(SPDR, buffer, 10);         //Convert from int to string
	USART_putstring(buffer);        //Send the converted value to the terminal
	USART_putstring("  ");
	USART_send('\r');
	USART_send('\n');               
}
