#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif /*__AVR_ATmega162__*/
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
#include "OLED_Driver.h"
#include "fonts.h"
#define FOSC 4915200UL
#define BAUD 9600UL
#define MYUBBR ((FOSC/(16*BAUD))-1)

#define EXIT_SUCCESS 0

int main(void)
{
	uint8_t calib_array[4];
	USART_Init(MYUBBR);
	USART_stream_setup();
	XMEM_init();
	SRAM_test();
	ADC_init();
	OLED_init();
	OLED_reset();
	ADC_calibrate(calib_array);
	int selector = 1;
	int game_done = 1;
	while (game_done) 
	{
		_delay_ms(300);
		joy_dir dirJoy = dir_read(calib_array);
		if(dirJoy==UP){
			selector -= 1;
			if(selector < 1){
				selector=4;
			}
			OLED_clear_column(0, 10);
			OLED_pos(selector+2, 0);
			OLED_printf_char('>');
		}
		else if (dirJoy==DOWN) {
			selector += 1;
			if (selector>4) {
				selector = 1;
			}
			OLED_clear_column(0, 10);
			OLED_pos(selector+2, 0);
			OLED_printf_char('>');
		}
		OLED_home(selector);
	}

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
		printf("Joystick direction:%s\n\r", joy_dir_to_string(dirJoy));
		printf("Slider right raw:%d\t", pos[2]);
		printf("Slider left raw:%d\n\r", pos[3]);
		printf("\n\r");
		_delay_ms(1000);
	}
	return EXIT_SUCCESS;
}
