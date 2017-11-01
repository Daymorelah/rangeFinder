# include <avr/io.h>
# define F_CPU 16000000UL
# include <util/delay.h>
# include "LCD2.h"


char line_position [4] = {0, 64, colum, (64+colum) };
char alfa_numeric [8];

void busy_check(){
	dataDDR = 0x00 ; // set port as input
	controlPort |= (1<<RW) ; // Enable read mode on R/W
	controlPort &= ~(1<<RS) ; // Enable command mode by RS being off
	if (pinBF & (1<<7)){
		kick() ; // on and off enable pin
	} // end of if statement
	dataDDR = 0xFF ; // set data pot as output
}// end of busy check
void kick(){
	controlPort |= (1<<Enable) ;
	asm volatile ("nop") ;
	asm volatile ("nop") ;
	controlPort &= ~(1<<Enable) ;
}// end of function kick
void commmand (unsigned char command){
	busy_check() ;
	dataPort = command ;
	controlPort &= ~ ((1<<RS) | (1<<RW));
	kick() ;
	dataPort = 0x00 ;
	} // end of function command
void character(unsigned char charater) {
	busy_check() ;
	dataPort = charater ;
	controlPort |= (1<<RS) ;
	controlPort &= ~(1<<RW) ;
	kick() ;
	dataPort = 0x00 ; // make data port input
	} // end of function character
void send_string(char *strings) {
	while (*strings > 0)
	{
		character(*strings++) ;
		_delay_ms(300) ;
	}
	} // end of function send_string
void send_variable(int value) {
	itoa(value, alfa_numeric, 10) ; // converts integer to string
	send_string(alfa_numeric); // outputs the value
	} // end of function send variable
void send_variable_double(int value, char width, char precision) {
	dtostrf(value, width, precision, alfa_numeric) ; // converts double to string
	send_string(alfa_numeric) ; // outputs string variable to the LCD
	} // end of function send variable double
void position_row_col(char x, char y) {
	commmand(0x80 + line_position[x-1] + (y-1) ) ; // goes to position specifies by X and Y
} // end of function position_row_col
