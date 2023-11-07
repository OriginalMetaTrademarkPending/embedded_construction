#include "dac.h"
#include "component/component_dacc.h"
#include "instance/instance_dacc.h"
#include "instance/instance_pmc.h"

	// PINS FOR MJ1 og MJ2
			// !OE = PIN25 = PD0
			// SEL = PIN27 = PD2
			// DIR = PIN32 = PD10
			// NOT_RST = PIN26 = PD1
			// EN = PIN30 = PD9;

			// D00 = PIN33 = PC1
			// D01 = PIN34 = PC2
			// DO2 = PIN35 = PC3
			// DO3 = PIN36 = PC4
			// DO4 = PIN37 = PC5
			// DO5 = PIN38 = PC6
			// DO6 = PIN39 = PC7
			// DO7 = PIN40 = PC8


int16_t max_right = 0;
int16_t max_left = 0;


int dac_init()
{
	REG_PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | ID_DACC << PMC_PCR_PID_Pos;
	REG_PMC_PCER1 |= 1<<(ID_DACC-32);
	REG_DACC_CR = 1;
	// Configuring the DAC module
	REG_DACC_MR = (DACC_MR_TRGEN_DIS) | (DACC_MR_WORD_HALF) | (DACC_MR_USER_SEL_CHANNEL1) | (DACC_MR_STARTUP_0);
	REG_DACC_IER = DACC_IER_EOC;
	// Enabling channel 1 for the DAC
	REG_DACC_CHER = DACC_CHER_CH1;

	return 0;
}

void dac_write(uint16_t data)
{
	REG_DACC_CDR = DACC_CDR_DATA(data);
	while(!(DACC->DACC_ISR & DACC_ISR_EOC));
}

void motor_driver()
{
	REG_PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PIOC << PMC_PCR_PID_Pos);
	REG_PMC_PCER0 |= 1<<ID_PIOC;
	REG_PIOC_PER = PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5 | PIO_PC6 | PIO_PC7 | PIO_PC8;
	REG_PIOC_ODR = PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5 | PIO_PC6 | PIO_PC8 | PIO_PC8;

	// Defining the necessary pins for communicating with the motor box.
	PIOD->PIO_PER = (NOT_OE | SEL | DIR | NOT_RST | EN);
	PIOD->PIO_OER = (NOT_OE | SEL | DIR | NOT_RST | EN);
	
	// Set NOT_OE to high to stop sampling immediately.
	// Set EN to high to disable brakes.
	PIOD->PIO_SODR = NOT_RST | EN;
	PIOD->PIO_CODR = NOT_OE | SEL | DIR;
	reset_motor();
}

int16_t motor_read()
{
	// 1. Set NOT_OE low to sample and hold the encoder value;
	// 2. Set SEL low to output high byte
	PIOD->PIO_CODR = (NOT_OE | SEL);
	// 3. Wait approximately 20 us for output to settle.
	_delay_us(20);
	// 4. Read MJ2 to get high byte
	uint8_t high = (REG_PIOC_PDSR & ENC_MSK) >> 1;
	// 5. Set SEL high to output low byte
	PIOD->PIO_SODR = (SEL);
	// 6. Wait approximately 20 microseconds
	_delay_us(20);
	// 7. Read MJ2 to get low byte
	uint8_t low = (REG_PIOC_PDSR & ENC_MSK) >> 1;
	// 8. Set NOT_OE to high
	PIOD->PIO_SODR = (NOT_OE);
	uint16_t result = high << 8 | low;
	if (result & (1<<15))
	{
		return ((uint16_t)(~result) + 1);
	}
	printf("Value read from motor: %u\n\r", result);
	return result;
}

void reset_motor()
{
	PIOD->PIO_CODR = NOT_RST;
	_delay_us(20);
	PIOD->PIO_SODR = NOT_RST;
}

uint16_t joy_map(uint8_t pos)
{
    return (uint16_t)(pos*((max_right)-(max_left)/100)+max_left);
}

void encoder_calibrate()
{
	reset_motor();
	int16_t enc = motor_read();
	dac_write(10);
	PIOD->PIO_CODR = DIR;
	_delay_ms(1000);
	while(abs(enc - motor_read()) > 100)
	{
		enc = motor_read();
		_delay_ms(1000);
	}
	max_left = enc;
	PIOD->PIO_SODR = DIR;
	_delay_ms(1000);
	while(abs(enc - motor_read()) > 100)
	{
		enc = motor_read();
		_delay_ms(1000);
	}
	max_right = enc;
	printf("left calib: %d\n\r", max_left);
    printf("right calib: %d\n\r", max_right);
}