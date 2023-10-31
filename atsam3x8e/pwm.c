#include "pwm.h"
#include "component/component_pmc.h"
#include "component/component_pwm.h"
#include "instance/instance_pioc.h"
#include "instance/instance_pmc.h"
#include "instance/instance_pwm.h"
#include "pio/pio_sam3x8e.h"
#include "sam3x8e.h"
#include <stdint.h>

int pwm_init()
{
	/*TODO:
	*	Set clock for pwm module.
	*	Set waveform period.
	*	Set waveform duty cycle.
	*/
	
// 	// Activate peripheral B for PWMH5 and PWMH6 
// 	PIOC->PIO_ABSR |= PIO_PC19B_PWMH5 | PIO_PC18B_PWMH6;
// 	// Disable PIO control from PWMH5 and PWMH6
// 	PIOC->PIO_PDR |= PIO_PC19B_PWMH5 | PIO_PC18B_PWMH6;
// 	// Divide master clock by 42. Result -> 2MHz
// 	PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PWM << PMC_PCR_PID_Pos);
// 	// Activate clock for peripheral 36 (PWM).
// 	PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);
	
// 	REG_PWM_CMR5 = PWM_CMR_CALG | PWM_CMR_CPOL | PWM_CMR_CPRE_CLKA;
// 	REG_PWM_CPRD5 = 20000;
// //	REG_PWM_CMR6 = PWM_CMR_CALG | PWM_CMR_CPOL | PWM_CMR_CPRE_CLKB;

// //	// Set PWM clock of A and B to 1 MHz, by setting prescaler to 84.
// //	PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84);
// //	PWM->PWM_CLK = PWM_CLK_PREB(0) | PWM_CLK_DIVB(84);
// //
// //	PWM->PWM_CH_NUM[5].PWM_CPRD = PWM_CPRD_CPRD(20000);
// //	PWM->PWM_CH_NUM[6].PWM_CPRD = PWM_CPRD_CPRD(0);
// //
// 	REG_PWM_CDTY5 = 1500;

// 	// Enable channel 5 and 6
// 	PWM->PWM_ENA = PWM_ENA_CHID5;

	REG_PMC_PCER1 |= PMC_PCER1_PID36;
	REG_PIOC_PDR |= PIO_PDR_P19;
	REG_PIOC_ABSR |= PIO_ABSR_P19;
	REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42);
	REG_PWM_CMR5 = PWM_CMR_CALG | PWM_CMR_CPOL | PWM_CMR_CPRE_CLKA;
	REG_PWM_CPRD5 = 20000;
	REG_PWM_CDTY5 = 1500;
	REG_PWM_ENA = PWM_ENA_CHID5;
	return 0;
}

void pwm_set_dc(int value)
{
//	// Limit the value of dc (duty cycle) to something between 0,9 ms and 2,1 ms.
//	if(*duty_cycle > 0.105)
//	{
//		*duty_cycle = 0.105;
//	}
//	else if(*duty_cycle < 0.045)
//	{
//		*duty_cycle = 0.045;
//	}
//	int CDTY = (int)(PWM->PWM_CH_NUM[channel].PWM_CPRD*(*duty_cycle));
//	PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = PWM_CDTYUPD_CDTYUPD(CDTY);

	if(value < 900)
	{
		value = 1100;
	}
	else if(value > 2100)
	{
		value = 1900;
	}
	REG_PWM_CDTY5 = value;
}

