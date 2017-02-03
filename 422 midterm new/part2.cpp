#include<avr/io.h>
#define F_CPU 1000000UL //1 MHz 
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

bool lightOn = false;
int intensityA = 0;
int	intensityB = 0;
int ADCCounter = 0;
int maxCount = 0;

//Initialization for LEDs
void initLED()
{
	DDRD |= (1 << PD5) | (1 << PD6); //set PD5 and PD6 to output
}

//Initialization for the ADC
void initADC()
{
	ADMUX |= (1 << REFS0); //Voltage reference is AVcc, multiplexer is set to read pin PC0
	ADCSRA |= (1 << ADEN); //This enables the ADC 
}

//Method which reads from an analog source and returns an int
int readADC()
{
	ADCSRA |= (1 << ADSC); //Start read
	while(!(ADCSRA & (1<<ADIF))); //Do nothing while data is being read
	ADCSRA |= (1 << ADIF); //reset interrupt flag
	return (ADC); //return the read value
}

//Initialization for PWM
void initPWM()
{	
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1<< WGM00); //8 bit clock
	TCCR0B |= (1 << CS00); //No prescaler, start the clock
	TCNT0 = 0; //start timer
	OCR0A = OCR0B = 0;
}

//initialization for analog read's clock
void initBlinkTimer()
{
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B |= (1 << WGM12)|(1 << CS12); // set up timer with prescaler = 256 and CTC mode
    TCNT1H = 0;
	TCNT1L = 0;	// initialize counter
	OCR1A = 0;
	TIMSK1 = (1 << OCIE1A);   //enable interrupt for compare
}

void initADCTimer()
{
	TCCR2A = 0;
	TCCR2B = 0;
	TCCR2A |= (1 << WGM21); //set timer to CTC
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);//set prescaler to 1024
	TCNT2 = 0;
	OCR2A = 98; //this is roughly 0.1 seconds
	TIMSK2 = (1 << OCIE2A);
}

void stopTimer()
{
	TIMSK1 = (0 << OCIE1A);
	OCR1A = OCR1B = 0;
	OCR0A = OCR0B = 0;
	TCNT0 = TCNT1H = TCNT1L = 0;
}


//Initialization for serial communication
void initUsart()
{
	UCSR0B |= (1 << RXCIE0) | (1 << RXEN0)  | (1 << TXEN0); //Enable RX, TX
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); //This sets data size to 8 bit
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
}

//Returns a char that was typed/ received
char receiveByte()
{
	while( !( UCSR0A & ( 1 << RXC0 ) )  ){};
	return UDR0;
}

//Returns a char that was transmitted
void transmitByte(char data)
{
	while( !( UCSR0A & ( 1 << UDRE0 ) ) ){};
	UDR0 = data;
}

//Method to print text, it is done by transmitting each char of a string
void printStr(char const*s)
{
	while(*s)
	{
		transmitByte(*s);
		s++;
	}
}

//Method that processes what user has typed and returns it as an int
int getInput()
{	
	int bufferIndex = 0; //tracks the current index
	char givenByte; //whatever the user typed
	char myBuffer[128]; //the string that we are building
	int input; //the number that will be returned
	while(1)
	{	
		givenByte = receiveByte(); //call method receive byte to get the typed byte
		if(givenByte == 0x0D) //if enter was sent
		{
			if(bufferIndex == 0) //if the buffer is empty
				continue; //do nothing and restart the loop
			
			myBuffer[bufferIndex++] = '\n'; //newline
			myBuffer[bufferIndex++] = '\r'; //carriage return
			myBuffer[bufferIndex] = 0; //null-terminate the chars, this turns it into a string
			input = atoi(myBuffer); //turn the string into an int
			printStr(myBuffer); //print what was in the buffer so user can see what they entered
			return input;
		}
		else if(isdigit(givenByte)) //if a number was sent
		{
			myBuffer[bufferIndex] = givenByte;//add it to the buffer
			bufferIndex++; //increment the index
		}
		else //if something other than a number or enter was sent
		{
			printStr("Please enter a number.\n\r"); //message the user and ask them to enter a number
		}
		
	}
}

//similar to get input, but just for the LED
int getLED()
{
	int bufferIndex = 0;
	char givenByte;
	char myBuffer[128];
	int input;
	while(1)
	{	
		givenByte = receiveByte();
		if(givenByte == 0x0D) //if enter was sent
		{
			if(bufferIndex == 0)
				continue;
		
			myBuffer[bufferIndex++] = '\n'; //newline
			myBuffer[bufferIndex++] = '\r'; //carriage return
			myBuffer[bufferIndex] = 0; //null-terminate the chars, this turns it into a string
			input = atoi(myBuffer); //turn the string into an int
			printStr(myBuffer); //print what was in the buffer so user can see what they entered
			return input;
		}
		else if(givenByte == '1' | givenByte == '2') //if 1 or 2 was typed
		{
			if(bufferIndex < 1) //if there is nothing in the buffer
			{
				myBuffer[bufferIndex] = givenByte;
				bufferIndex++;	
			}
			else//if there is already something in the buffer
			{
				printStr("Press Enter!\n\r");//it should already be a 1 or 2, so we ask the user to press enter
			}
		}
		else
		{
			printStr("Please enter 1 or 2.\n\r");//ask user to enter 1 or 2 
		}
		
	}
}

//method that blinks the LEDs according to how the user specified.
void blink(int dca, int dcb)
{
	if(lightOn == false)
	{
		OCR0A = dca; //LED1 will get duty cycle a
		OCR0B = dcb; //LED2 will get duty cycle b
		lightOn = true;
	}
	else
	{
		OCR0A = 0; //turn off both LEDs
		OCR0B = 0;
		lightOn = false;
	}
}

void getReads()
{
	char tempbuff[128];
	int tempIndex = 0;
	int holder = 0;
	
	holder = readADC();
	sprintf(tempbuff, "%d\n\r", holder);
	printStr(tempbuff);
}

int main(void)
{
	int currentLED = 0;
	int givenDelay = 0;
	int readDelay = 0;
	
	//initializers
	initADC();
	initADCTimer();
	initLED();
	initUsart();
	initBlinkTimer();
	initPWM();
	
	//set all the outputs to high
	PORTD |= (1 << PD5) | (1 << PD6);
		
	printStr("Hello, welcome to LED blinker!\n\r");	
	
	printStr("Please press 1 or 2 to pick an LED.\n\r");
	currentLED = getLED();

	printStr("Please enter a number from between 0 to 255 for the intensity.\n\r");
	if(currentLED == 1)
		intensityA = getInput();
	else if (currentLED == 2)
		intensityB = getInput();
	
	printStr("Please enter a number from between 0 to 255 for the intensity of the other LED.\n\r");
	if(currentLED == 1)
		intensityB = getInput();
	else if (currentLED == 2)
		intensityA = getInput();
	
	printStr("Please enter a number from between 100 to 2000 for a delay in milliseconds.\n\r");
	givenDelay = getInput();
	
	printStr("Please enter a number from 0 to 15 for the interval of reads.\n\r");
	readDelay = getInput();
	
	OCR1A = givenDelay * 4.2; //4200 is roughly 1 sec, I multiplied by 4200 and then divided by 1000, which gives 4.2
	maxCount = readDelay * 10;
	
	sei();
	TCNT1H = 0;
	TCNT1L = 0;
	
	while(1) //main loop
	{	

	}
    
    return 0;
}


ISR(TIMER1_COMPA_vect)  // timer1 compare interrupt
{
	
	cli();
	blink(intensityA,intensityB);
	sei();	
}

ISR(TIMER2_COMPA_vect)  // timer2 compare interrupt
{
	ADCCounter++;
	if(ADCCounter >= maxCount)
	{	
		getReads();
		ADCCounter = 0;
	}	
}