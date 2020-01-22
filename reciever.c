/*
 * testversion.c
 *
 * Created: 1/5/2020 10:17:47 PM
 * Author : Srikar Kandimalla
 */ 

#include <avr/io.h>
#include <stdio.h>
#define F_CPU 16000000
#define FOSC 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <stdlib.h>
#include "usart.h"
#include "SPI.h"
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


int main(void)
{
	int arr[1];
	
	uint8_t length,temp;
	USART_Init(MYUBRR);
	DDRC = 0x00;
	PORTC = 0x00;
	SPIInitiateLora(SPI_CLOCK_DIV64);
	
	readRegister(RegVersion);
	printf("REG VERSION : %02x\n",SPDR);
	
	readRegister(RegOpMode);
	printf("REG OPMODE  : %02x\n",SPDR);
	
	writeRegister(RegOpMode,0x00);
	writeRegister(RegOpMode,0x80);
	writeRegister(RegOpMode,0x81);
	
	readRegister(RegOpMode);
	printf("REG OPMODE  : %02x\n",SPDR);
	if(SPDR == 0x81)
	{
		printf("LORA IS IN STANDBY MODE\n");
	}
	writeRegister(RegFrMsb,0xD9);
	writeRegister(RegFrMib,0x00);
	writeRegister(RegFrLsb,0x00);
	
	writeRegister(RegPaConfig,0x06);
	
	writeRegister(RegPayloadLength,0xFF);
	
	readRegister(RegPayloadLength);
	printf("REG PAYLOADLENGTH : %02x\n",SPDR);
	
	for(int i=0;i<100;i++)
	{
		writeRegister(RegOpMode,0x83);
		_delay_ms(3);
		readRegister(RegIrqFlags);
		printf("sync running\n");
		_delay_ms(3);
	}

	

    /* Replace with your application code */
    while (1) 
    {
		writeRegister(RegOpMode,0x85);
		
		_delay_ms(2000);
		
		readRegister(RegRxNbBytes);
		printf("REG RXNBBYTES : %02x\n",SPDR);
		length = SPDR;
		
		readRegister(RegIrqFlags);
		if((SPDR & 0x40) == 0x00)
		break;
		else if((SPDR & 0x40) == 0x40)
		{
			
			readRegister(RegIrqFlags);
			printf("REG IRQFLAGS : %02x\n",SPDR);
			
			writeRegister(RegOpMode,0x81);
			
			readRegister(RegOpMode);
			printf("REG OPMODE : %02x\n",SPDR);
			
			writeRegister(RegIrqFlags,0x50);
			
			readRegister(RegFifoRxCurrentAddr);
		
			temp = SPDR;
			writeRegister(RegFifoAddrPtr,temp);
			
			for(int i=0;i<1;i++)
			{
				readRegister(RegFifo);
				arr[i] = SPDR;
				printf("\nWATER_LEVEL : %02x\n",arr[0]);
			}
			
			
			
			
		}
	    
		
	}
}

