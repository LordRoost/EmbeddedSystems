//slave
#include <avr/io.h>
#include <stdlib.h>
#define F_CPU 1000000UL // Set CPU speed for delay
#include <util/delay.h>
#define BAUDRATE 4800
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

uint16_t adc_value;                      //Variable used to store the value read from the ADC
void adc_init(void);                     //Function to initialize/configure the ADC
uint16_t read_adc(uint8_t channel);      //Function to read an arbitrary analogic channel/pin


void adc_init(void){
 ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    //16Mhz/128 = 125Khz the ADC reference clock
 ADMUX |= (1<<REFS0);                             //Voltage reference from Avcc (5v)
 ADCSRA |= (1<<ADEN);                             //Turn on ADC
 ADCSRA |= (1<<ADSC);                             //Do an initial conversion
}

uint16_t read_adc(uint8_t channel){
 ADMUX &= 0xF0;                  //Clear the older channel that was read
 ADMUX |= channel;               //Defines the new ADC channel to be read
 ADCSRA |= (1<<ADSC);            //Starts a new conversion
 while(ADCSRA & (1<<ADSC));      //Wait until the conversion is done
 return ADCW;                    //Returns the ADC value of the chosen channel
}

void SPISlaveSend(uint8_t data){   //slave send function
  PORTB &= ~(1<<PB2);               //select slave
  SPDR=data;                        //send data
  while (!(SPSR &(1<<SPIF)));       //wait for transmition complete
  PORTB |= (1<<PB2);                //SS to high
}

//Function to send and receive data
unsigned char spi_tranceiver (unsigned char data)
{
    SPDR = data;                       //Load data into the buffer
    while(!(SPSR & (1<<SPIF) ));       //Wait until transmission complete
    return(SPDR);                      //Return received data
}


void SPISlaveInit(void){            //SPI init
  DDRB |= (1<<PB4);                 //set MISO as output
  SPCR = (1<<SPE)|(1<<SPIE);        //enable SPI and enable SPI interrupt
}

int main(void){
	unsigned char data;
  adc_init();                       //Setup the ADC
  SPISlaveInit();      //initialize slave SPI
  while (1){                        //initial PWM value
    adc_value = read_adc(0);        //Read ADC0
	data = spi_tranceiver(adc_value);
    //SPISlaveSend(adc_value);       //send ADC0 value to slave
    _delay_ms(500);
  }
}