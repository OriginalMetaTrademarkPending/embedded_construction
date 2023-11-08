#include "motor.h"

int16_t max_right;
int16_t max_left;

void motor_init()
{
    // Activating the clock for the MJ2 pins.
    REG_PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PIOC << PMC_PCR_PID_Pos);
	REG_PMC_PCER0 |= 1<<ID_PIOC;
    // Disabling the output mode of the MJ2 pins.
	REG_PIOC_PER |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5 | PIO_PC6 | PIO_PC7 | PIO_PC8;
	REG_PIOC_ODR |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5 | PIO_PC6 | PIO_PC8 | PIO_PC8;
	// Defining the necessary pins for communicating with the motor box.
	PIOD->PIO_PER |= (NOT_OE | SEL | DIR | NOT_RST | EN);
	PIOD->PIO_OER |= (NOT_OE | SEL | DIR | NOT_RST | EN);
	
    // Reset the internal counter.
    reset_motor();

	// Initial configuration.
    // Activate NOT_RST and enable pins
    // Deactivate NOT_OE, SEL and DIR.
	PIOD->PIO_SODR |= NOT_RST | EN;
	PIOD->PIO_CODR |= NOT_OE | SEL | DIR;
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
	int16_t result = (int16_t)(high << 8 | low);
	printf("Value read from motor: %u\n\r", result);
	return result;
}

void reset_motor()
{
	PIOD->PIO_CODR = NOT_RST;
	_delay_us(20);
	PIOD->PIO_SODR = NOT_RST;
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

uint16_t joy_map(uint8_t pos)
{
    return (uint16_t)(pos*((max_left - max_right)/100)+ max_right);
}