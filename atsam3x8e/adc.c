#include "adc.h"
#include "component/component_adc.h"
#include "component/component_pmc.h"
#include "pio/pio_sam3x8e.h"
#include "sam3x8e.h"

int adc_init()
{
	ADC->ADC_MR = ADC_MR_FREERUN;
	ADC->ADC_CHER = ADC_CHER_CH7;

	PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_ADC << PMC_PCR_PID_Pos);
	PMC->PMC_PCER1 |= 1 << (ID_ADC - 32);

	ADC->ADC_CR = ADC_CR_START;
	return 0;
}

uint16_t adc_read()
{
	return ADC->ADC_CDR[0];
}

uint16_t ma_read()
{
	uint16_t measurement = adc_read();
	sum -= readings[ma_index];
	readings[ma_index] = adc_read();
	sum += measurement;
	ma_index++;
	if(ma_index == WINDOW_SIZE)
	{
		ma_index = 0;
	}
	return (sum / WINDOW_SIZE);
}