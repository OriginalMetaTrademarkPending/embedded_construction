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
#include "STATEMACHINE.h"
#define FOSC 4915200UL
#define BAUD 9600UL
#define MYUBBR ((FOSC/(16*BAUD))-1)

#define EXIT_SUCCESS 0

int main(void)
{
	USART_Init(MYUBBR);
	XMEM_init();
	OLED_init();
	//SRAM_test();
	ADC_init();
	uint8_t calib_array[4];
	ADC_calibrate(calib_array);
	FSM_loop(calib_array);
	return EXIT_SUCCESS;
}
