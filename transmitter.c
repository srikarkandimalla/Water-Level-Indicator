/*
 * Tx.c
 *
 * Created: 12/20/2019 9:27:59 PM
 * Author : Srikar Kandimalla
 */ 

#include <avr/io.h>
#include <stdio.h>
#define F_CPU 16000000
#define FOSC 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <math.h>
#include <avr/power.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <stdlib.h>
#include "usart.h"
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

volatile int cnt;
ISR(TIMER1_OVF_vect)
{
	cnt++;
}




int main(void)
{
	int arr[1];
	int dist,height_tank=10;
	uint16_t x;
	uint8_t var1,water_level;
	USART_Init(MYUBRR);
	DDRC = 0x00;
	PORTC = 0x00;
	DDRD =0xFF;
	TIMSK1 = (1<<TOIE1);
	TCCR1A = 0x00;
	DDRB |= (1<<PORTB5)|(1<<PORTB3)|(1<<PORTB2);
	DDRB &= ~(1<<PORTB4);
	PORTB |= (1<<PORTB2);
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR1);
	/****************************************************************/
	/********  Check Register Version *******************************/
	/********************************************************/
	cli();
	SPDR = 0x42;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0xDD;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
	printf("VERSION : %02x\n",SPDR);
	if(SPDR == 0x22)
	printf("SPI SUCCESS\n");
	
	
	
	/*******************************************************************/
	/************ check for operating mode ***************************************/
	/*******************************************************************/
	cli();
	SPDR = 0x01;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0xDD;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
	printf("\nOPMODE : %02x\n",SPDR);
	
	
	cli();
	SPDR = 0x81;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x00;                     //switch to sleep mode
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
	
	cli();
	SPDR = 0x81;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x80;                    //switch to lora sleep mode
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
	
	cli();
	SPDR = 0x81;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x81;
	_NOP();                        //switch to lora standby mode
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
	
	cli();
	SPDR = 0x01;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0xDD;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
	printf("\nswitching to LORASTBY MODE\n");
	printf("OPMODE : %02x\n",SPDR);
	
	/*******************************************/
	/***** TX INIT ****************************/
	/*******************************************/
/************************************************/	
	//carrier frequency = 868.00MHz
	cli();
	SPDR = 0x86;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0xD9;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x00;                          //BURST Access
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x00;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
/*****************************************************/	
//	REG PACONFIG
	cli();
	SPDR = 0x89;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x06;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
/*****************************************************/	
//REG PARAMP DEFAULT	
	cli();
	SPDR = 0x0A;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0xDD;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	printf("RegPAramp : %02x\n",SPDR);
	sei();
//REG OCP DEFAULT	
	cli();
	SPDR = 0x0B;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0xDD;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
	
	printf("REGPINC : %02x\n",PINC);
	printf("RegOCP : %02x\n",SPDR);
//REG LNA DEFAULT	
	cli();
	SPDR = 0x0C;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0xDD;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
	printf("RegLna : %02x\n",SPDR);
	
//RegModemConfig1 Default	
	cli();
	SPDR = 0x1D;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0xDD;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
	printf("RegModemConfig1 : %02x\n",SPDR);
	
//RegModemConfig2 Default	
	cli();
	SPDR = 0x1E;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0xDD;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
	printf("RegModemConfig2 : %02x\n",SPDR);
/*************  REG DIOMAPPING1 *********************/
    /*cli();
	SPDR = 0xC0;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x40;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<SPIF);
	sei(); */   
/***************  REG FIFO TX BASE ADDRESS **********/	
	cli();
	SPDR = 0x8E;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x00;                     //fifotxbaseaddr = 0x00
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
/***** REG FIFO ADDRPNTR ******/	
	cli();
	SPDR = 0x8D;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x00;                    // fifoaddrpntr = 0x00
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
//Read RegTxBaseAddr	
	cli();
	SPDR = 0x0E;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0xDD;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
	printf("RegTxBaseAdrr : %02x\n",SPDR);
//Read RegFifoAddrPntr	
	cli();
	SPDR = 0x0D;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0xDD;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
	printf("RegFifoAddrPntr : %02x\n",SPDR);
	
	cli();
	SPDR = 0x8F;
	PORTB &= ~(1<<PORTB2);
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = 0x00;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<PORTB2);
	sei();
	
	
    /* Replace with your application code */
    while (1) 
    {/********************************/
	 /*** CAD INIT ******************/
	 
	 SPDR = 0x81;
	 PORTB &= ~(1<<PORTB2);
	 _NOP();
	 while(!(SPSR & (1<<SPIF)));
	 SPDR = 0x87;
	 _NOP();
	 while(!(SPSR & (1<<SPIF)));
	 PORTB |= (1<<PORTB2);
	 sei();
	 
	 _delay_ms(2000);
	 //Read RegIrqFlags After Cad;
	 cli();
	 SPDR = 0x12;
	 PORTB &= ~(1<<PORTB2);
	 _NOP();
	 while(!(SPSR & (1<<SPIF)));
	 SPDR = 0xDD;
	 _NOP();
	 while(!(SPSR & (1<<SPIF)));
	 PORTB |= (1<<PORTB2);
	 sei();
	 printf("RegIrqFlagsAfterCAD : %02x\n",SPDR);
	 var1 = SPDR;
	 //clear CADDONE interrupt
	 cli();
	 SPDR = 0x92;
	 PORTB &= ~(1<<PORTB2);
	 _NOP();
	 while(!(SPSR & (1<<SPIF)));
	 SPDR = 0x40;
	 _NOP();
	 while(!(SPSR & (1<<SPIF)));
	 PORTB |= (1<<PORTB2);
	 sei();
	 if((var1 & 0x01) == 0x01)
	 {
		 //clear CADDETECTED interrupt
		 cli();
		 SPDR = 0x92;
		 PORTB &= ~(1<<PORTB2);
		 _NOP();
		 while(!(SPSR & (1<<SPIF)));
		 SPDR = 0x01;
		 _NOP();
		 while(!(SPSR & (1<<SPIF)));
		 PORTB |= (1<<PORTB2);
		 sei();
		 while(1)
		 {
			 
		  /******FIND DIST USING ULTRASONIC SENSOR ********/
			DDRB &= ~(1<<DDB0);
			PORTB |= (1<<PORTB0);  
			
			_delay_ms(50);
			
			PORTD |= (1<<PORTD7);
			_delay_us(15);
			PORTD &= ~(1<<PORTD7);
			
			TCNT1 = 0x0000;
			TCCR1B = 0x41;
			TIFR1 = (1<<ICF1);
			TIFR1 = (1<<TOV1);
			
			while((TIFR1 & (1<<ICF1)) == 0);
			TCNT1 = 0x0000;
			TCCR1B = 0x01;
			TIFR1 = (1<<ICF1);
			TIFR1 = (1<<TOV1);
			cnt  = 0;
			
			while((TIFR1 & (1<<ICF1)) == 0);
			x = ICR1 + (65535*cnt);
			dist = x/941.17;
			water_level = (height_tank - dist);
			_delay_ms(200);
			
			if((water_level>=0) && (water_level<400))
			{
				    for(int i=0;i<1;i++)
					{
						arr[i] = water_level;
					}
				
			    	/***************  REG FIFO TX BASE ADDRESS **********/
			    	cli();
			    	SPDR = 0x8E;
			    	PORTB &= ~(1<<PORTB2);
			    	_NOP();
			    	while(!(SPSR & (1<<SPIF)));
			    	SPDR = 0x00;                     //fifotxbaseaddr = 0x00
			    	_NOP();
			    	while(!(SPSR & (1<<SPIF)));
			    	PORTB |= (1<<PORTB2);
			    	sei();
			    	/***** REG FIFO ADDRPNTR ******/
			    	cli();
			    	SPDR = 0x8D;
			    	PORTB &= ~(1<<PORTB2);
			    	_NOP();
			    	while(!(SPSR & (1<<SPIF)));
			    	SPDR = 0x00;                    // fifoaddrpntr = 0x00
			    	_NOP();
			    	while(!(SPSR & (1<<SPIF)));
			    	PORTB |= (1<<PORTB2);
			    	sei();
			    	//Read RegTxBaseAddr
			    	cli();
			    	SPDR = 0x0E;
			    	PORTB &= ~(1<<PORTB2);
			    	_NOP();
			    	while(!(SPSR & (1<<SPIF)));
			    	SPDR = 0xDD;
			    	_NOP();
			    	while(!(SPSR & (1<<SPIF)));
			    	PORTB |= (1<<PORTB2);
			    	sei();
			    	printf("RegTxBaseAdrr : %02x\n",SPDR);
			    	//Read RegFifoAddrPntr
			    	cli();
			    	SPDR = 0x0D;
			    	PORTB &= ~(1<<PORTB2);
			    	_NOP();
			    	while(!(SPSR & (1<<SPIF)));
			    	SPDR = 0xDD;
			    	_NOP();
			    	while(!(SPSR & (1<<SPIF)));
			    	PORTB |= (1<<PORTB2);
			    	sei();
			    	printf("RegFifoAddrPntr : %02x\n",SPDR);
					
					//write data to be transmitted in to fifo databuffer
					cli();
					SPDR = 0x80;
					PORTB &= ~(1<<PORTB2);
					_NOP();
					for(int i=0;i<1;i++)
					{
						while(!(SPSR & (1<<SPIF)));
						SPDR = arr[i];//Data to be transmitted
						_NOP();
						_NOP();
						_NOP();
					}
				    while(!(SPSR & (1<<SPIF)));
					PORTB |= (1<<PORTB2);
					sei();
					
					//Read fifoaddrpntr after writing fifo databuffer
					cli();
					SPDR = 0x0D;
					PORTB &= ~(1<<PORTB2);
					_NOP();
					while(!(SPSR & (1<<SPIF)));
					SPDR = 0xDD;
					_NOP();
					while(!(SPSR & (1<<SPIF)));
					PORTB |= (1<<PORTB2);
					sei();
					printf("\nAfter Writing Fifo\nRegFifoAddrPntr : %02x\n",SPDR);
					
					//write RegPayloadLength 
					cli();
					SPDR = 0xA2;
					PORTB &= ~(1<<PORTB2);
					_NOP();
					while(!(SPSR & (1<<SPIF)));
					SPDR = 0x01;                 //RegPayladLength = 1BYTE
					_NOP();
					while(!(SPSR & (1<<SPIF)));
					PORTB |= (1<<PORTB2);
					sei();
					 
					cli();
					SPDR = 0x22;
					PORTB &= ~(1<<PORTB2);
					_NOP();
					while(!(SPSR & (1<<SPIF)));
					SPDR = 0xDD;
					_NOP();
					while(!(SPSR & (1<<SPIF)));
					PORTB |= (1<<PORTB2);
					sei();
					printf("RegPayloadLength : %02x\n",SPDR);
					
					//START TRANSMISSION
					SPDR = 0x81;
					PORTB &= ~(1<<PORTB2);
					_NOP();
					while(!(SPSR & (1<<SPIF)));
					SPDR = 0x83;                   //Tx mode
					_NOP();
					while(!(SPSR & (1<<SPIF)));
					PORTB |= (1<<PORTB2);
					sei();
					
					 _delay_ms(3);
					//Read RegIrqFlags AfterTransmission 
					cli();
					SPDR = 0x12;
					PORTB &= ~(1<<PORTB2);
					_NOP();
					while(!(SPSR & (1<<SPIF)));
					SPDR = 0xDD;
					_NOP();
					while(!(SPSR & (1<<SPIF)));
					PORTB |= (1<<PORTB2);
					sei();
					printf("RegIrqFlagsAftertransmission : %02x\n\n\n",SPDR);
					//clear TXDone interrupt
					cli();
					SPDR = 0x92;
					PORTB &= ~(1<<PORTB2);
					_NOP();
					while(!(SPSR & (1<<SPIF)));
					SPDR = 0x08;
					_NOP();
					while(!(SPSR & (1<<SPIF)));
					PORTB |= (1<<PORTB2);
					sei();
					

			}
		 }
		 
		 
		 
    
	 
	 
	 
	 
		 
	 				
				                    
									 
							
	 
								
	 
	 
	 
	 
										
	                               
	  	
    }
}
}
