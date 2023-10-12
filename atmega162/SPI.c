#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#include <stdint.h>
#endif /* __AVR_ATmega162__*/
#include "SPI.h"

void SPI_init()
{
	// Set MOSI, SS and SCK as output, all others as input
	DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS);
	PORTB |= (1<<SS);
	// Enable SPI as master, set clock rate fck/16
	SPCR = (1<<SPIE)|(1<<SPE)|(1<<MSTR)|(1<<SPR0);

	// Disable speed doubler
	SPSR &= ~(1<<SPI2X);
}

void SPI_transmit(uint8_t data)
{
	// Start transmission
	SPDR = data;
	// Wait for transmission completed
	while(!(SPSR&(1<<SPIF)));
}

uint8_t SPI_receive(void)
{
	// Transmit dummy data to slave
	SPDR = 0xFF;
	// Wait for transmission completed
	while(!(SPSR&(1<<SPIF)));
	// Read the data
	return(SPDR);
}
