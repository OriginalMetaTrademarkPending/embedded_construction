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
	uint8_t calib_array[4];
	uint8_t adc_meas[4];
	USART_Init(MYUBBR);
	XMEM_init();
	CAN_init(MODE_NORMAL);
	ADC_init();
	OLED_init();
	ADC_calibrate(calib_array);
	printf("ADC calibrated! Sending data...\n\r");
	while(1)
	{
		ADC_read(0, adc_meas);
		ADC_send_data(adc_meas);
	}
	return EXIT_SUCCESS;
}
