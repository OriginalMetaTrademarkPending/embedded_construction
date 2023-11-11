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
#include "MENU.h"
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

	//OLED_test();
	fsm_transition_to(STATE_MENU, calib_array);
//	USART_Init(MYUBBR);
//	ADC_init();
//	uint8_t result[4];
//	while(1)
//	{
//		ADC_read(0, result);
//		printf("Slider left: %u\n\r", result[2]);
//		printf("Slider right: %u\n\r", result[3]);
//		if((PINB & (1<<PB3)) == (1<<PB3))
//		{
//			printf("Button 3 pressed\n\r");
//		}
//		if((PINB & (1<<PB4)) == (1<<PB4))
//		{
//			printf("Button 4 pressed\n\r");
//		}
//		if(!((PINB & (1<<PB2)) == (1<<PB2)))
//		{
//			printf("Joystick button pressed\n\r");
//		}
//		_delay_ms(500);
//	}
	return EXIT_SUCCESS;
}
