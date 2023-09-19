#include "ADC_Driver.h"
#include "XMEM_Driver.h"
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#define ADC_BASE_ADDR 0x1400

#define F_CPU 4915200
#include <util/delay.h>

void ADC_init()
{
	DDRD |= (1<<PD4);
	TCCR3A &= ~(1<<COM3A0);
	TCCR3A |= (1<<COM3A1 | 1<<WGM31);
	TCCR3B &= ~(1<<WGM33 | 0b111<<CS30);
	TCCR3B |= (1<<WGM32 | 1<<WGM33 | 0b01 << CS30);
	ICR3 = 20;
	while(TCNT3 < 10);
	OCR3A = 10;
}

void ADC_read(uint8_t channel, uint8_t* result)
{
	/* Defining the base ADC address. */
	volatile uint8_t* adc_addr = (uint8_t*)0x1400;

	/* Sending the channel value as a bitmask to PORTA. */
	adc_addr[0] = channel;

	/* Delaying for read completion. */
	_delay_us(20);

	for(uint8_t i = 0; i < 4; i++)
	{
		result[i] = adc_addr[i];
	}
}

void ADC_calibrate(uint8_t* calib_array)
{
	uint8_t pos[4];
	uint32_t max_time = 100;
	ADC_read(0, pos);
	uint8_t mid_valX = pos[0];
	uint8_t mid_valY = pos[1];
	uint8_t minX = mid_valX;
	uint8_t maxX = mid_valX;
	uint8_t minY = mid_valY;
	uint8_t maxY = mid_valY;
	printf("Calibrating: Move the joystick around for 10 seconds...\n\r");
	for(uint32_t i = 0; i < max_time; i++)
	{
		ADC_read(0, pos);
		volatile uint8_t measX = pos[0];
		volatile uint8_t measY = pos[1];
		if(measX < minX)
		{
			minX = measX;
			calib_array[0] = minX;
		}
		if(measX > maxX)
		{
			maxX = measX;
			calib_array[1] = maxX;
		}
		if(measY < minY)
		{
			minY = measY;
			calib_array[2] = minY;
		}
		if(measY > maxY)
		{
			maxY = measY;
			calib_array[3] = maxY;
		}

		_delay_ms(100);
	}
	if ((maxX-minX)!=(mid_valX))
	{
		if ( (maxX-mid_valX) < (mid_valX - minX))
		{
			minX = 2*mid_valX - maxX;
			calib_array[0] = minX;
		}
		else 
		{
			maxX = 2*mid_valX - minX;
			calib_array[1] = maxX;
		}
	}
	if ((maxY-minY)!=(mid_valY))
	{
		if ( (maxY-mid_valY) < (mid_valY - minY))
		{
			minY = 2*mid_valY - maxY;
			calib_array[2] = minY;
		}
		else 
		{
			maxY = 2*mid_valY - minY;
			calib_array[3] = maxY;
		}
	}
}

pos_t pos_read(uint8_t* calib_array)
{
	pos_t result;
	uint8_t pos[4];
	ADC_read(0, pos);
	uint8_t x_min = calib_array[0];
	uint8_t x_max = calib_array[1];
	uint8_t y_min = calib_array[2];
	uint8_t y_max = calib_array[3];

	uint8_t x_raw = pos[0];
	uint8_t y_raw = pos[1];

	if (x_raw<x_min)
	{
		x_raw = x_min;
	}
	if (x_raw>x_max)
	{
		x_raw = x_max;
	}
	if (y_raw<y_min)
	{
		y_raw=y_min;
	}
	if (y_raw>y_max)
	{
		y_raw=y_max;
	}

	

	int8_t percX = (int8_t)(((x_raw-x_min)*(200.0))/(x_max-x_min)) - 100;
	int8_t percY = (int8_t)(((y_raw-y_min)*(200.0))/(y_max-y_min)) - 100;
	result.posX_t = percX;
	result.posY_t = percY;

	return result;
}

joy_dir dir_read(uint8_t* calib_array)
{
	pos_t joy_pos = pos_read(calib_array);

	if(abs(joy_pos.posX_t) > abs(joy_pos.posY_t))
	{
		if(joy_pos.posX_t >= 20)
		{
			return RIGHT;
		}
		else if(joy_pos.posX_t <= -20)
		{
			return LEFT;
		}
		else
		{
			return NEUTRAL;
		}
	}else
	{
		if(joy_pos.posY_t >= 20)
		{
			return UP;
		}
		else if(joy_pos.posY_t <= -20)
		{
			return DOWN;
		}
		else
		{
			return NEUTRAL;
		}

	}
}
