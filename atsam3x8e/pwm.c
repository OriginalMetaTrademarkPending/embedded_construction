#include "pwm.h"
#include "component/component_pmc.h"
#include "component/component_pwm.h"
#include "instance/instance_pioc.h"
#include "instance/instance_pmc.h"
#include "instance/instance_pwm.h"
#include "pio/pio_sam3x8e.h"
#include "sam3x8e.h"
#include <stdint.h>

void pwm_init()
{
	REG_PMC_PCER1 |= PMC_PCER1_PID36;
	REG_PIOC_PDR |= PIO_PDR_P19;
	REG_PIOC_ABSR |= PIO_ABSR_P19;
	REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42);
	REG_PWM_CMR5 = PWM_CMR_CALG | PWM_CMR_CPOL | PWM_CMR_CPRE_CLKA;
	REG_PWM_CPRD5 = 20000;
	REG_PWM_CDTY5 = 1500;
	REG_PWM_ENA = PWM_ENA_CHID5;
}

void pwm_set_dc(int value)
{
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

