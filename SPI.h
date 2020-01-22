/*
 * SPI.h
 *
 * Created: 1/6/2020 5:13:38 PM
 *  Author: Srikar Kandimalla
 */ 


#ifndef SPI_H_
#define SPI_H_

#define RegVersion				0x42
#define RegDioMapping1			0x40
#define RegOpMode				0x01
#define RegFrMsb				0x06
#define RegFrMib				0x07
#define RegFrLsb				0x08
#define RegPaConfig				0x09
#define RegPaRamp				0x0A
#define RegOcp					0x0B
#define RegLna					0x0C
#define RegModemConfig1			0x1D
#define RegModemConfig2			0x1E
#define RegFifoAddrPtr			0x0D
#define RegPayloadLength		0x22
#define RegIrqFlags				0x12
#define RegRxNbBytes			0x13
#define RegFifo					0x00
#define RegFifoTxBaseAddr		0x0E
#define RegFifoRxBaseAddr		0x0F
#define RegFifoRxCurrentAddr	0x10

#define SPI_CLOCK_DIV4			0x00			// clock Div = 0;
#define SPI_CLOCK_DIV16			0x01			// clock Div = 1;
#define SPI_CLOCK_DIV64			0x02			// clock Div = 2;
#define SPI_CLOCK_DIV128		0x03			// clock Div = 3;
#define SPI_CLOCK_DIV2			0x04			// clock Div = 4;
#define SPI_CLOCK_DIV8			0x05			// clock Div = 5;
#define SPI_CLOCK_DIV32			0x06			// clock Div = 7;

#define SPI_CLOCK_MASK			0x03			// SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK		0x01			// SPI2X = bit 0 on SPSR

#define DDR_SPI  DDRB
#define SPI_PORT PORTB
#define DD_SS	 DDB2
#define DD_MOSI  DDB3
#define DD_MISO  DDB4
#define DD_SCK   DDB5
#define SS	     PORTB2
#define MOSI     PORTB3
#define MISO     PORTB4
#define SCK      PORTB5

void SPIInitiateLora(uint8_t clockdiv){
	power_spi_enable();
	uint8_t spcr,spsr;
	spcr = (1<<SPE)|(1<<MSTR)|((clockdiv) & SPI_CLOCK_MASK);
	spsr = ((clockdiv>>2) & SPI_2XCLOCK_MASK);
	
	DDR_SPI |= (1<<DD_SS)|(1<<DD_MOSI)|(1<<DD_SCK);
	DDR_SPI &= ~(1<<DD_MISO);
	
	SPI_PORT |= (1<<SS);
	
	SPCR = spcr;
	SPSR = spsr;
	printf("Settings of SPCR : %02x\n",SPCR);
	printf("Settings of SPSR : %02x\n",SPSR);
	
}

inline uint8_t readRegister(uint8_t address){
	cli();
	SPI_PORT &=~(1<<SS);
	SPDR = address;
	_NOP();
	while(!(SPSR & _BV(SPIF)));
	SPDR = 0xDD;
	_NOP();
	while(!(SPSR & _BV(SPIF)));
	SPI_PORT |= (1<<SS);
	sei();
	
	return SPDR;
}
inline void writeRegister(uint8_t address,uint8_t data){
	cli();
	address |= (0x80);
	SPI_PORT &=~(1<<SS);
	SPDR = address;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPDR = data;
	_NOP();
	while(!(SPSR & (1<<SPIF)));
	SPI_PORT |= (1<<SS);
	sei();
}








#endif /* SPI_H_ */