#include "dac.h"
#include "component/component_dacc.h"
#include "instance/instance_dacc.h"
#include "instance/instance_pmc.h"

void dac_init()
{
	REG_PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | ID_DACC << PMC_PCR_PID_Pos;
	REG_PMC_PCER1 |= 1<<(ID_DACC-32);
	REG_DACC_CR = 1;
	// Configuring the DAC module
	REG_DACC_MR = (DACC_MR_TRGEN_DIS) | (DACC_MR_WORD_HALF) | (DACC_MR_USER_SEL_CHANNEL0) | (DACC_MR_STARTUP_0);
	REG_DACC_IER = DACC_IER_EOC;
	// Enabling channel 0 for the DAC
	REG_DACC_CHER = DACC_CHER_CH0;
}

void dac_write(uint16_t data)
{
	REG_DACC_CDR = DACC_CDR_DATA(data);
	while(!(DACC->DACC_ISR & DACC_ISR_EOC));
}
