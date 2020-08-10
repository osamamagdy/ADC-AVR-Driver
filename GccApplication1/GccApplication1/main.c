/*
 * GccApplication1.c
 *
 * Created: 7/19/2020 8:17:38 PM
 * Author : Osama Magdy
 */ 


/////////// My Answer using tutorial steps /////////

#include <avr/io.h>
#include "lcd.h"
#include "myutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ADC_VREF_TYPE 0x00
// REFS1:0 ---> 0 0   for using the AREF value
// ADLAR -----> 0   for 10-bit precision mode



int main()
{
	float vol1,vol2;
	char p1[4] ;
	char p2[4];
	
	//Initializing LCD
	InitLCD(LS_BLINK);
	
	//Configuring pins as inputs
	DDRA = 0x00;
	
	
	// Choosing REFS1:0 ---> 0 0 to make us use AREF
	// And 10-bit precision mode
	ADMUX = ADC_VREF_TYPE;
	
	
	// Enabling the ADC module itself
	// Choosing Prescaler to 2
	ADCSRA = (1<<ADPS1) | (1<<ADEN);
	
	LCDWriteString("First : ");
	LCDWriteStringXY(0,1,"Second : ");


	while (1)
	{
		// Reading Channel 1
		//First, enable the channel (ADC0--> MUX4:0 = 00000)
		// Note : if you made it --> ADMUX|= MUX0 , this will not set the ADMUX to 00000000 but it will or the value from previous loop with 0.
		// This why at every time we change the ADMUX, we use the following :
		ADMUX= ADC_VREF_TYPE | MUX0;
		
		//Second, delay for 10 microseconds
		_delay_us(10);
		
		//Third, Start ADC conversion
		ADCSRA |= (1<<ADSC);
		
		//As long as the flag is zero, we enter the loop
		while( ( ADCSRA & (1<<ADIF) ) ==0 );
		
		//Setting the flag to one
		ADCSRA |= (1<<ADIF);
		vol1 =  ADCL;
		vol1 += (ADCH<<8);
		
		vol1 = vol1 * 5 / 1023.0;
		/*
		5 volts ----> 1023 
		? -----> x 
		? = x*5/1023.0
		*/
		
		// Convert to string ( dtostrf(the float variable , number of characters for the whole value including the decimal point , number of digits after the decimal point , the *char we print)  )
		dtostrf(vol1,4,2,p1);
		// Note : in this particular example i can exceed the size of the string array without any warnings or errors by writing dtostrf(x,6,5,str); ,
		// but if i changed "char p[4]" to any smaller size it gives dozens of errors. I have no idea why is that.
		
		LCDWriteStringXY(8,0,p1);
		// Printing the string value
		
		
		
		
		/// Reading Channel 2
		
		//First, enable the channel (ADC1--> MUX4:0 = 00001)
		ADMUX = ADC_VREF_TYPE | (1<<MUX0);
		
		//Second, delay for 10 microseconds
		_delay_us(10);
		
		//Third, Start ADC conversion
		ADCSRA |= (1<<ADSC);
		
		//As long as the flag is zero, we enter the loop
		while( ( ADCSRA & (1<<ADIF) ) ==0 );
		
		//Setting the flag to one
		ADCSRA |= (1<<ADIF);
		vol2 =  ADCL;
		vol2 += (ADCH<<8);
		
		vol2 = vol2 * 5 / 1023.0;
		/*
		5 volts ----> 1023 
		? -----> x 
		? = x*5/1023.0
		*/
		
		// Convert to string ( dtostrf(the float variable , number of characters for the whole value including the decimal point , number of digits after the decimal point , the *char we print)  )
		dtostrf(vol2,4,2,p2);
		// Note : in this particular example i can exceed the size of the string array without any warnings or errors by writing dtostrf(x,6,5,str); ,
		// but if i changed "char p[4]" to any smaller size it gives dozens of errors. I have no idea why is that.
		
		LCDWriteStringXY(9,1,p2);
		// Printing the string value
		
		
		
		
	}
	
	return 0 ;

}




//////////// Tutorial Answer with function ///////////////
/*
#define ADC_VREF_TYPE 0x20
// REFS1:0 ---> 0 0   for using the AREF value
// ADLAR -----> 1   for 8-bit precision mode
 
unsigned char read_adc (unsigned char adc_input)
{
	ADMUX = adc_input | (ADC_VREF_TYPE);
	// Choosing the channel we read from, and choosing 8-bit precision mode by using ADC_VREF_TYPE
	_delay_us(10);
	// Necessary time for the ADC module
	
	ADCSRA |= 0x40;
	// Setting the ADC start conversion bit to 1
	
	while((ADCSRA & 0x10)==0);
	//As long as the flag is zero, we enter the loop
	
	
	ADCSRA |= 0x10;
	//Returning the flag to 1
	return ADCH;
}



int main(void)
{
	float x;
	char str[4];
	
	ADMUX = ADC_VREF_TYPE;
	// Choosing REFS1:0 ---> 0 0 to make us use AREF
	// And 8-bit precision mode
	
	
	ADCSRA = 0x81;
	// Enabling the ADC module itself
	// Choosing Prescaler to 2
	
	
	
	InitLCD(LS_BLINK);
	LCDWriteString("First : ");
	// LCD initialization
	
	
	while(1)
	{
		// Read inputs
		x=read_adc(0x00);
		x=x*5/255.0;
		
		//5 volts ----> 1023 
		//? -----> x 
		//? = x*5/1023.0
		//but as we use 8-bit precision mode 
		//? = ?*4
		//so, ?=x*5/255.0'''
		
		
		
		
		// Convert to string ( dtostrf(the float variable , number of characters for the whole value including the decimal point , number of digits after the decimal point , the *char we print)  )
		dtostrf(x,4,2,str);
		// Note : in this particular example i can exceed the size of the string array without any warnings or errors by writing dtostrf(x,6,5,str); ,
		// but if i changed "char str[4]" to any smaller size it gives dozens of errors. I have no idea why is that.
		
		LCDWriteStringXY(8,0,str);
		// Printing the string value
		

	}
	
	
}

*/



/////////////My first Wrong Answer//////////////////////////////
/*
InitLCD(LS_BLINK);
DDRA = 0x00;
ADCSRA = 0x00;
ADCSRA |=0x80;
ADMUX = 0x00;
int vol1,vol2;
char * p;
LCDWriteString("First : ");


while (1)
{
	ADMUX |= 0x08;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA & (1<<ADIF))==0);
	vol1 = ADCH;
	vol1=(vol1<<8)|ADCL;
	vol1=(vol1*5000)/1024;
	LCDGotoXY(8,0);
	LCDWriteInt(vol1,6);
	
}
*/