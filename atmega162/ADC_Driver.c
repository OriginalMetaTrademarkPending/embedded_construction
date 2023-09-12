#include "ADC_Driver.h"
#include "XMEM_Driver.h"
#include <stdint.h>
#include <stdio.h>

void ADC_init()
{
	/* Set the prescaler to 1. */
	CLKPR &= ~(0xFF);
	CLKPR |= (1<<CLKPCE);
	CLKPR = 0x00;

	/* Configuring timer 0 for generation of clock signal. */
	ASSR |= (1<<AS2);
}

void ADC_test()
{
	// Define some address in the ADC address group.
	volatile char* adc_addr = (char*) 0x1400;
	uint16_t adc_addr_size = 0x3FF;
	uint16_t write_errors = 0;
	uint16_t read_errors = 0;
	printf("Starting ADC test...");
	uint16_t seed = rand();
	srand(seed);
	for(uint16_t i = 0; i < adc_addr_size; i++)
	{
		uint8_t some_val = rand();
		adc_addr[i] = some_val;
		uint8_t retrieved_val = adc_addr[i];
		if(retrieved_val != some_val)
		{
			printf("Write phase error: adc_addr[%4d] = %02X (should be %02X)\n\r", i, retrieved_val, some_val);
			write_errors++;
		}
	}
	srand(seed);
	for(uint16_t i = 0; i < adc_addr_size; i++)
	{
		uint8_t some_val = rand();
		uint8_t retrieved_val = adc_addr[i];
		if(retrieved_val != some_val)
		{
			printf("Retrieve phase error: adc_addr[%4d] = %02X (should be %02X)\n\r", i, retrieved_val, some_val);
			read_errors++;
		}
	}
	printf("ADC test completed with \n\r%4d errors in write phase and \n\r%4d errors in read phase\n\n\r", write_errors, read_errors);
}
