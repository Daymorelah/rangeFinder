/*
 * Range_finder.cpp
 *
 * Created: 2/12/2016 9:48:14 AM
 *  Author: HUSSAIN ADEMOLA
 */ 

/*
This program interfaces an ultrasonic sensor or range finder. An LCD (16x2) shows the distance being measured. it uses the LCD library.
The program uses external interrupt, INT1, feature, this is located at PD2 on atmega32. the prog. runs on a 16MHz crystal, and uses a 
Pre scaler of 64, thus the counter takes 4 micro seconds to increment and each instruction takes 62.5 nanoseconds to execute. The external
interrupt uses the "interrupt on any logic change" to read the time the echo pin is held high. the program also uses timer0 output compare
register to make trigger high for 16 micro seconds, after which its cleared low. PortA is used for data lines of the LCD, portD3 for the  
INT0 (i.e Echo pin, and its declared as input), PortD0,1,2, for RS,RW and Enable pin of the LCD respectively, PortC1 is used for the trigger 
pin. The next assignment is to interface more than one range finder.  
*/

# include <avr/io.h>
# include "LCDisplay/LCD2.h"
# define F_CPU 16000000UL
# include <util/delay.h>
# include <avr/interrupt.h>

static volatile int y, x = 0;
static int dist = 0 ;

int main(void)
{
	
	DDRC = 0x02 ; // set pin1 as output and pin0 as input
	DDRD |= 0b1110111; // set pD2 as input. this is d INT0 pin
	GICR |= (1<<INT1); // enable external interrupt
	MCUCR |= ((0<<ISC11)|(1<<ISC10)) ; // enable interrupt on any logic change on INT0
	OCR0 = 4 ; // output compare register
	TIMSK |= (1<<OCIE0) ; // enable output compare interrupt
	commmand(0x38);
	_delay_ms(500);
	commmand(0x0F);
	sei(); // enable global interrupt
	
	while(1)
    {
		TCCR0 |= ((1<<CS01)|(1<<CS00)|(1<<WGM01)); // set counter0	
		PORTC |=(1<<1); // make trig pin high
			send_string("The Dist. is:");
			_delay_ms(1000);
			dist = y*0.068;
			send_variable(dist);
			_delay_ms(500);
			send_string("cm");
			_delay_ms(1000);
	       commmand(0x01);
		   _delay_ms(500);
		   commmand(0x02);
		   _delay_ms(500);
	} // end of while loop 
} //  end of function main
	
	ISR(INT1_vect){
		if (x==1)
		{
			TCCR1B = 0 ; // disable counter
			y = TCNT1 ; // assign count1 value to variable y
			TCNT1 = 0 ; // initialize counter
			x = 0;
		} // end high to low operation
		if (x==0)
		{
			TCCR1B |= ((1<<CS11)|(1<<CS10));
			x =1 ;
		} // end of low to high operation
		}// end of interrupt service 


ISR(TIMER0_COMP_vect){
		PORTC &= ~(1<<1); // make trig pin low
	TCCR0 = 0x00 ; // disable timer0	
	}// end of interrupt service routine

/*int main(void){
	controlDDR = 0x0F ;
	while(1){
		commmand(0x38);
		_delay_ms(500);
		commmand(0x0E);
		_delay_ms(500);
		commmand(0x01);
		_delay_ms(500);
		character('H');
		_delay_ms(500);
		character('e');
		_delay_ms(1000);
		position_row_col(2,2);
		_delay_ms(500);
		send_string(" welcome back ");
		_delay_ms(2000);
	} // end of while loop
}// end of function main*/			