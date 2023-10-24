#include "pwm.h"
#include "component/component_pmc.h"
#include "pio/pio_sam3x8e.h"
#include "sam3x8e.h"

int pwm_init()
{
	/*TODO:
	*	Set clock for pwm module.
	*	Set waveform period.
	*	Set waveform duty cycle.
	*/
	
	// Activate peripheral B for PWMH5 and PWMH6 
	PIOC->PIO_ABSR |= PIO_PC19B_PWMH5 | PIO_PC18B_PWMH6;
	// Disable PIO control from PWMH5 and PWMH6
	PIOC->PIO_PDR |= PIO_PC19B_PWMH5 | PIO_PC18B_PWMH6;
	// Divide master clock by 42. Result -> 2MHz
	PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PWM << PMC_PCR_PID_Pos);
	PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);

	// Center align channel 5. This means that the duty cycle uses half the period.
	REG_PWM_CMR5 = PWM_CMR_CALG | PWM_CMR_CPOL | PWM_CMR_CPRE_CLKA;
	// Defining the channel period
	REG_PWM_CPRD5 = 20000;
	// Defining the channel duty cycle
	REG_PWM_CDTY5 = 1500;
	// Enabling the channel
	REG_PWM_ENA = PWM_ENA_CHID5;

	return 0;
}

void pwm_set_dc(int value)
{
	// Limiting to min 0,9 ms and max 2,1 ms
	if(value < 900){
		value = 900;
	}
	else if(value > 2100)
	{
		value = 2300;
	}
	// Writing the value to the duty cycle
	REG_PWM_CDTY5 = value;
}

