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
	SRAM_test();
	ADC_init();
	uint8_t calib_array[4];
	ADC_calibrate(calib_array);
	uint8_t pos[4];
	while(1)
	{
		ADC_read(0, pos);
		int8_t posX = pos_read(calib_array).posX_t;
		int8_t posY = pos_read(calib_array).posY_t;
		joy_dir dirJoy = dir_read(calib_array);
		printf("Joystick x raw:%d\t", pos[0]);
		printf("Joystick x percent:%d\n\r", posX);
		printf("Joystick y raw:%d\t", pos[1]);
		printf("Joystick y percent:%d\n\r", posY);
		printf("Joystick direction:%d\n\r", dirJoy);
		printf("Slider right raw:%d\t", pos[2]);
		printf("Slider left raw:%d\n\r", pos[3]);
		printf("\n\r");
		_delay_ms(1000);
	}
	return EXIT_SUCCESS;
}
