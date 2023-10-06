#include "ADC.h"

/**
 * TODO: Find out the reason for line 37...
 */
void ADC_init()
{
	DDRD |= (1<<PD4);				//PWM signal for clocking the ADC comes from D4.
	TCCR3A &= ~(1<<COM3A0);				//Clear the previous Compare Output Mode configuration.
	
	/*
	 * New Compare Output Mode configuration.
	 *
	 * Set the Waveform Generation Mode bit 1 to 1.
	 * Set the Compare Output Mode bit 1 to 1.
	 */
	TCCR3A |= (1<<COM3A1 | 1<<WGM31);

	TCCR3B &= ~(1<<WGM33 | 0b111<<CS30);		//Clear the previous Waveform Generation Mode and Clock Prescaler settings.
	
	/*
	 * New Waveform Generation Mode configuration.
	 *
	 * Set the Waveform Generation Mode bits 2 and 3 to 1.
	 * Set the Clock Prescaler to 001 (no prescaling).
	 */
	TCCR3B |= (1<<WGM32 | 1<<WGM33 | 0b01 << CS30);
	ICR3 = 20;					//Using the Input Capture Register 3 for setting the TOP value of the counter.
	while(TCNT3 < 10);				//??
	OCR3A = 10;					//Setting the Output Compare Register 3 to 10 (half of TOP value).
}


/**
 * A lot to go through here...
 * First and foremost, a clear decision hasn't been made regarding the read strategy.
 * 
 * INPUT/OUTPUT MODE vs. HARDWIRED MODE:
 * Personally, the input/output mode sound much better on paper and is quite interesting in the long run.
 * By writing the necessary channel that we need, we can effectively use the extra XMEM.
 * However the configuration is more tricky and the results might be problematic.
 *
 * Hardwired Mode on the other hand is quite straightforward and is set up through wiring.
 * It allows us to read the entire memory block written by the ADC which consists of 4 bytes.
 * However, we cannot access the channels directly, and we need to somehow pass the result array around
 * the entire runtime of the function...
 *
 * The implementation below needs to change to accomodate the following points:
 *
 * 1) Usage of the BUSY pin (mandatory): We want to set up an ISR to take over the reading after the conversion process.
 *	This makes the reading much cleaner than just waiting for 20 microseconds, big optimization in runtime.
 * 2) Move to input/output mode (optional): We would appreciate a more configurable solution to the given problem, but
 *	this is not a mandatory requirement, more of an efficient design question.
 */
void ADC_read(uint8_t channel, uint8_t* result)
{
	/* Defining the base ADC address. */
	volatile uint8_t* adc_addr = (uint8_t*)0x1400;

	/* Sending the channel value as a bitmask to PORTA. */
	adc_addr[0] = channel;

	/* Delaying for read completion. */
	_delay_us(20);

	/* Read all the measurements from the ADC. */
	for(uint8_t i = 0; i < 4; i++)
	{
		result[i] = adc_addr[i];
	}
}

/**
 * This part of the code needs to be cleaned up a bit, there are many variables in the stack...
 * I assume it can be cleaner regarding performance, but in the way it is setup we would reduce
 * readability and maintainability greatly.
 */
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

/*
 * Not an elegant solution, it works but still...ugly.
 */
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

/*
 * Don't think there's a better way of doing this unfortunately...thresholding looks like the best way.
 */
joy_dir dir_read(pos_t* position)
{
	if(abs(position->posX_t) > abs(position->posY_t))
	{
		if((position->posX_t) >= 20)
		{
			return RIGHT;
		}
		else if((position->posX_t) <= -20)
		{
			return LEFT;
		}
		else
		{
			return NEUTRAL;
		}
	}else
	{
		if((position->posY_t) >= 20)
		{
			return UP;
		}
		else if((position->posY_t) <= -20)
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
	switch(direction)
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
	XMEM_init();
	ADC_init();
	uint8_t calib_array[4];
	ADC_calibrate(calib_array);
	uint8_t pos[4];
	while(1)
	{
		ADC_read(0, pos);
		pos_t position = pos_read(calib_array);
		int8_t posX = position.posX_t;
		int8_t posY = position.posY_t;
		joy_dir dirJoy = dir_read(&position);
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
}
