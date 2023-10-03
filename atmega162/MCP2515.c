#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#include "SPI.h"
#include <stdint.h>
#endif /*__AVR_ATmega162__*/
#include "mcp2515_defs.h"
#include "MCP2515.h"

int MCP2515_init()
{
	uint8_t value;

	SPI_init();		// Initialize SPI
	MCP2515_reset();

	//Self-test
	MCP2515_read(MCP_CANSTAT, &value);
	if((value & MODE_MASK) != MODE_CONFIG)
	{
		printf("MCP2515 is NOT in configuration mode after reset!\n\r");
		return -1;
	}

	MCP2515_write(MCP_CANCTRL, MODE_LOOPBACK);
	MCP2515_read(MCP_CANSTAT, &value);
	if((value & MODE_MASK) != MODE_LOOPBACK)
	{
		printf("MCP2515 is NOT in loopback mode after initialization!\n\r");
		return 1;
	}
	return 0;
}

void MCP2515_reset()
{
	PORTB &= ~(1<<SS);
	SPI_send(MCP_RESET);
	PORTB |= (1<<SS);
}

void MCP2515_read(uint8_t address, uint8_t* data)	
{
	PORTB &= ~(1<<SS);
	SPI_send(MCP_READ);			// Send read instructions
	SPI_send(address);			// Send address
	*data = SPI_read();			// Read result
	PORTB |= (1<<SS);
}

void MCP2515_write(uint8_t address, uint8_t data)
{
	PORTB &= ~(1<<SS);
	SPI_send(MCP_WRITE);
	SPI_send(address);
	SPI_send(data);
	PORTB |= (1<<SS);
}

void MCP2515_request_to_send(uint8_t channel)
{
	PORTB &= ~(1<<SS);
	SPI_send(channel);
	PORTB |= (1<<SS);
}

void MCP2515_read_status(uint8_t* data)
{
	PORTB &= ~(1<<SS);
	SPI_send(MCP_READ_STATUS);
	*data = SPI_read();
	PORTB |= ~(1<<SS);
}

void MCP2515_bit_modify(uint8_t address, uint8_t mask_byte, uint8_t data)
{
	PORTB &= ~(1<<SS);
	SPI_send(MCP_BITMOD);
	SPI_send(address);
	SPI_send(mask_byte);
	SPI_send(data);
	PORTB |= (1<<SS); 
}
