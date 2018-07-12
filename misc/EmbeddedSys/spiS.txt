//SPI slave
#include <avr/io.h>
#include <stdlib.h>
#define F_CPU 1000000UL   //Needs to be placed before util/delay
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUDRATE 4800
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

char buffer[5];                          //Output of the itoa function
void USART_init(void);                   //Function to initialize and configure the USART/serial
void USART_send( unsigned char data);    //Function that sends a char over the serial port
void USART_putstring(char* StringPtr);   //Function that sends a string over the serial port

void SPISlaveInit(void){            //SPI init
  DDRB |= (1<<PB4);                 //set MISO as output
  SPCR = (1<<SPE)|(1<<SPIE);        //enable SPI and enable SPI interrupt
}

void InitPort(void){                //initialize output port
  DDRD|=(1<<PD6);                   //set PD6 (OC0A) as output
}

void InitTimer0(void){              //Initialize Timer0
  TCNT0=0;                          //Set Initial Timer value
  OCR0A=0;                          //Place compare value to Output compare register
  //Set fast PWM mode
  //and make clear OC0A on compare match
  TCCR0A|=(1<<COM0A1)|(1<<WGM01)|(1<<WGM00);
}

void StartTimer0(void){
  TCCR0B|=(1<<CS01)|(1<<CS00);  //Set prescaller 64 and start timer
  }

ISR(SPI_STC_vect){
  USART_putstring("IR Sensor Reading: ");
  itoa(SPDR, buffer, 10);         //Convert the read value to an ascii string
  USART_putstring(buffer);        //Send the converted value to the terminal
  USART_putstring("  ");
  USART_send('\r');
  USART_send('\n');               // new line
}

void USART_init(void){              //initialize USART
 UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
 UBRR0L = (uint8_t)(BAUD_PRESCALLER);
 UCSR0B = (1<<RXEN0)|(1<<TXEN0);
 UCSR0C = (3<<UCSZ00);
}

void USART_send( unsigned char data){   //USART send to UDR0
 while(!(UCSR0A & (1<<UDRE0)));
 UDR0 = data;
}

void USART_putstring(char* StringPtr){    //send string as characters
  while(*StringPtr != 0x00){
   USART_send(*StringPtr);
   StringPtr++;
  }
}

int main(void){
  USART_init();        //Setup the USART
  SPISlaveInit();      //initialize slave SPI
  InitPort();
  InitTimer0();
  StartTimer0();
  sei();
  while (1){
    //loop
  }
}
