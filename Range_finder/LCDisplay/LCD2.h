/*
 * crystal_display.cpp
 *
 * Created: 1/30/2016 3:59:33 PM
 *  Author: LENOVO PC
 */ 
# ifndef LCD2
# define LCD2
# include <avr/io.h>
# include <stdlib.h>
# define F_CPU 16000000UL
# include <util/delay.h>
# define dataPort PORTA 
# define dataDDR DDRA
# define RS 0
# define RW 1
# define Enable 2
# define colum 16
# define controlPort PORTD
# define controlDDR DDRD
# define pinBF PINA

void kick() ;
void busy_check();
void commmand (unsigned char);
void character (unsigned char) ; 
void send_string (char*) ;
void send_variable (int);
void send_variable_double(int, char, char);
void position_row_col(char, char) ;

# endif
