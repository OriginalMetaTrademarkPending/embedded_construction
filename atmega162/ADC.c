#include "ADC.h"
#include "XMEM.h"
#include "CAN.h"
#include "UART.h"
#include "OLED.h"
#include <stdint.h>
#include "STATEMACHINE.h"

int ticks_passed = 0;

ISR(TIMER3_COMPA_vect)
{
	ticks_passed++;
}

void ADC_init()
{
	cli();
	XMEM_init();
	DDRD |= (1<<PD4);
	DDRB &= ~(1<<PB2);
	PORTB |= (1<<PB2);
	TCCR3A &= ~(1<<COM3A0);
	TCCR3A |= (1<<COM3A1 | 1<<WGM31);
	TCCR3B &= ~(1<<WGM33 | 0b111<<CS30);
	TCCR3B |= (1<<WGM32 | 1<<WGM33 | 0b01 << CS30);
	ETIMSK |= (1<<OCIE3A);
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
	_delay_us(15);

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
	//printf("Finished calibrating.\n\r");
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
	uint8_t slider_right_raw = pos[2];
	uint8_t slider_left_raw = pos[3];

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
	
	// Changed the formula. Now gives value from [0-100]. 50 means stable value.
	// New_value = (old_value-min_old)* ((max_new-min_new)/(max_old-min_old)) + min_new
	int8_t percX = (int8_t)(((x_raw-x_min)*(JOYSTICK_MAX - JOYSTICK_MIN))/(x_max-x_min)) + JOYSTICK_MIN;
	int8_t percY = (int8_t)(((y_raw-y_min)*(JOYSTICK_MAX - JOYSTICK_MIN))/(y_max-y_min)) + JOYSTICK_MIN;
	uint8_t percSlideRight = (uint8_t)(((slider_right_raw - SLIDER_MIN)*(SLIDER_MAX - SLIDER_MIN))/(ADC_MAX - ADC_MIN)) + SLIDER_MIN;
	uint8_t percSlideLeft = (uint8_t)(((slider_left_raw - SLIDER_MIN)*(SLIDER_MAX - SLIDER_MIN))/(ADC_MAX - ADC_MIN)) + SLIDER_MIN;
	result.posX_t = percX;
	result.posY_t = percY;
	result.slideLeft = percSlideLeft;
	result.slideRight = percSlideRight;
	return result;
}

joy_dir dir_read(pos_t joy_pos)
{
	if(abs(50-joy_pos.posX_t) > abs(50-joy_pos.posY_t))
	{
		if(joy_pos.posX_t >= 75)
		{
			return RIGHT;
		}
		else if(joy_pos.posX_t <= 25)
		{
			return LEFT;
		}
		else
		{
			return NEUTRAL;
		}
	}else
	{
		if(joy_pos.posY_t >= 75)
		{
			return UP;
		}
		else if(joy_pos.posY_t <= 25)
		{
			return DOWN;
		}
		else
		{
			return NEUTRAL;
		}

	}
}

const char* joy_dir_to_string(joy_dir direction)
{
	switch (direction)
	{
		case NEUTRAL: return "NEUTRAL";
		case LEFT: return "LEFT";
		case RIGHT: return "RIGHT";
		case UP: return "UP";
		case DOWN: return "DOWN";
	}
}

void ADC_test()
{
	ADC_init();
	uint8_t calib_array[4];
	ADC_calibrate(calib_array);
	uint8_t pos[4];
	while(1)
	{
		ADC_read(0, pos);
		pos_t position = pos_read(calib_array);
		joy_dir dirJoy = dir_read(position);
		printf("Joystick x raw:%d\t", pos[0]);
		printf("Joystick x percent:%d\n\r", position.posX_t);
		printf("Joystick y raw:%d\t", pos[1]);
		printf("Joystick y percent:%d\n\r", position.posY_t);
		printf("Joystick direction:%s\n\r", joy_dir_to_string(dirJoy));
		printf("Slider right raw:%d\t", pos[2]);
		printf("Slider left raw:%d\n\r", pos[3]);
		printf("\n\r");
		_delay_ms(1000);
	}
}

void ADC_send_data(pos_t *adc_meas)
{
	joy_dir joystick_direction = dir_read(*adc_meas);
	CAN_frame joy_data;
	joy_data.id = 0x0040;
	joy_data.frame_length = 5;
	joy_data.data[0] = adc_meas->posX_t;
	joy_data.data[1] = adc_meas->posY_t;
	joy_data.data[2] = adc_meas->slideRight;
	joy_data.data[3] = adc_meas->slideLeft;
	if(!(PINB & (1<<PB2)))
	{
		joy_data.data[4] = 0;
	}
	else
	{
		joy_data.data[4] = 1;
	}
	CAN_write(&joy_data);
	printf("Data sent: %u %u %u %u %u\n\r", joy_data.data[0], joy_data.data[1], joy_data.data[2], joy_data.data[3], joy_data.data[4]);
}

void ADC_send_subroutine(unsigned long myubbr)
{
	uint8_t calib_array[4];
	pos_t adc_meas;
	USART_Init(myubbr);
	XMEM_init();
	CAN_init(MODE_NORMAL);
	ADC_init();
	OLED_init();
	ADC_calibrate(calib_array);
	printf("ADC calibrated! Sending data...\n\r");
	while(1)
	{
		adc_meas = pos_read(calib_array);
		ADC_send_data(&adc_meas);
	}
}

void ADC_game(uint8_t *calib_array)
{
	CAN_init(MODE_NORMAL);
	pos_t adc_meas;
	//sei();
	while(!(PINB & (1<<PB1))) // TODO: Make a set aomunt of time in which this function runs
		{
			adc_meas = pos_read(calib_array);
			ADC_send_data(&adc_meas);
		}
	///cli();
}