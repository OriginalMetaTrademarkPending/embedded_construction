#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#include "mcp2515_defs.h"
#endif /*__AVR_ATmega162__*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#define F_CPU 4915200

#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "UART.h"
#include "XMEM.h"
#include "ADC.h"
#include "OLED.h"
#include "SPI.h"
#include "fonts.h"
#include "MCP2515.h"
#include "CAN.h"
#define FOSC 4915200UL
#define BAUD 9600UL
#define MYUBBR ((FOSC/(16*BAUD))-1)

#define EXIT_SUCCESS 0

int main(void)
{
	USART_Init(MYUBBR);
	CAN_init();
	CAN_frame data = {7, 6, "Hello!"};
	CAN_frame result = {};
	while(1)
	{
		CAN_write(&data);
		result = CAN_read();
		printf("ID: %d\n\rLength: %d\n\rData: %s\n\r", result.id, result.frame_length, result.data);
	}
	return EXIT_SUCCESS;
}
