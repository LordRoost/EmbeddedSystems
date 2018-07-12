#include<avr/io.h>
#define F_CPU 1000000UL 
#include <util/delay.h>
#include <string.h>
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)
 
int main(void)
{
	char received_byte; //The byte that is read and sent
	char message1[20] = "Hello there!"; //A greeting
	char message2[20] = "Oh hai dere!"; //Another greeting
	char message3[20] = "Greetings, wow."; //A third greeting

  
	UCSR0B |= (1<<RXEN0)  | (1<<TXEN0); //Initialization for serial communication
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
  
    for(;;)
	{
		// wait until a byte is ready to read
		while( ( UCSR0A & ( 1 << RXC0 ) ) == 0 ){}
  
		// grab the byte from the serial port
		received_byte = UDR0;
   
		// wait until the port is ready to be written to
		while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){}
 
		if(received_byte == '0') //If what was typed was 0
		{
			for(int i=0; i<strlen(message1); i++)//a loop to go through the entire array of chars that message 1 is comprised of
			{
				UDR0 = message1[i]; //send char by char
				_delay_ms(10); //need delay to print all
			
			}
			UDR0 = 0x0A; //new line
			UDR0 = 0x0D; //carraige return, aka make cursor go to front
		}
		else if(received_byte == '1') //If what was typed was 1
		{
			for(int i=0; i<strlen(message2); i++)
			{
				UDR0 = message2[i];
				_delay_ms(10);
				
			}
			UDR0 = 0x0A;
			UDR0 = 0x0D;
		}
		else //for anything else that is typed
		{
			for(int i=0; i<strlen(message3); i++)
			{
				UDR0 = message3[i];
				_delay_ms(10);
			}
			UDR0 = 0x0A;
			UDR0 = 0x0D;
		}

    }
    return 0;   /* never reached */
}