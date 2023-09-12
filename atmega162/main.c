#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#define F_CPU 4915200
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "UART_Driver.h"
#include "XMEM_Driver.h"
#include "ADC_Driver.h"

#define FOSC 4915200UL
#define BAUD 9600UL
#define MYUBBR ((FOSC/(16*BAUD))-1)

#define EXIT_SUCCESS 0

int main(void)
{
	USART_Init(MYUBBR);
	USART_stream_setup();
	XMEM_init();
	ADC_init();
	SRAM_test();
	ADC_test();
	return EXIT_SUCCESS;
}
