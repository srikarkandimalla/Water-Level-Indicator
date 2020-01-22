/*
 * usart.h
 *
 * Created: 11/3/2019 11:13:24 PM
 *  Author: Srikar Kandimalla
 */ 


#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <stdio.h>


static void USART_Init(unsigned int);
static int USART_Transmit(char , FILE* );
static int USART_Receive(FILE* );

static FILE mystdout;

void USART_Init(unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// Enable transmitter and receiver
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	fdev_setup_stream(&mystdout, USART_Transmit, USART_Receive, _FDEV_SETUP_RW);
	stdout = &mystdout;
	stdin = &mystdout;
}

// function to send data - NOT REQUIRED FOR THIS PROGRAM IMPLEMENTATION
inline int USART_Transmit(char data, FILE* stream)
{
	/* Wait for empty transmit buffer */
	if (data == '\n')
	USART_Transmit('\r',stream);
	
	while ( !( UCSR0A & (1<< UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
	return 0;
}

// function to receive data
inline int USART_Receive(FILE* stream)
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	
	/* Get and return received data from buffer */
	return UDR0;
}





#endif /* USART_H_ */