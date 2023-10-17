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

	_delay_ms(1);
	//Self-test
	value = MCP2515_read(MCP_CANSTAT);
	if((value & MODE_MASK) != MODE_CONFIG)
	{
		printf("MCP2515 is NOT in configuration mode after reset!\n\r");
		return -1;
	}

	MCP2515_write(MCP_CANCTRL, MODE_LOOPBACK);
	value = MCP2515_read(MCP_CANSTAT);
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
	SPI_transmit(MCP_RESET);
	PORTB |= (1<<SS);
}

uint8_t MCP2515_read(uint8_t address)	
{
	uint8_t result;
	PORTB &= ~(1<<SS);
	SPI_transmit(MCP_READ);			// Send read instructions
	SPI_transmit(address);			// Send address
	result = SPI_receive();			// Read result
	PORTB |= (1<<SS);
	return result;
}

void MCP2515_write(uint8_t address, char data)
{
	PORTB &= ~(1<<SS);
	SPI_transmit(MCP_WRITE);
	SPI_transmit(address);
	SPI_transmit(data);
	PORTB |= (1<<SS);
}

void MCP2515_request_to_send(uint8_t tx_buffer)
{
	PORTB &= ~(1<<SS);
	SPI_transmit(tx_buffer);
	PORTB |= (1<<SS);
}

uint8_t MCP2515_read_status()
{
	uint8_t result;
	PORTB &= ~(1<<SS);
	SPI_transmit(MCP_READ_STATUS);
	result = SPI_receive();
	PORTB |= (1<<SS);
	return result;
}

void MCP2515_bit_modify(uint8_t address, uint8_t mask_byte, char data)
{
	PORTB &= ~(1<<SS);
	SPI_transmit(MCP_BITMOD);
	SPI_transmit(address);
	SPI_transmit(mask_byte);
	SPI_transmit(data);
	PORTB |= (1<<SS); 
}
