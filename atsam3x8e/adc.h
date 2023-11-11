#ifndef ADC_H
#define ADC_H
#include "sam.h"
#include "sam3x8e.h"
#include "component/component_adc.h"
#include "component/component_pmc.h"
#define WINDOW_SIZE 5

static int ma_index = 0;
static int sum = 0;
static int readings[WINDOW_SIZE];

/**
 * @brief Initializes the ADC module.
 */
void adc_init();

/**
 * @brief Reads from the ADC module.
 * @return The value read from the ADC module.
 */
uint16_t adc_read();

/**
 * @brief Filters the measurement.
 * @return A filtered value read from the ADC module.
*/
uint16_t ma_read();
#endif /*ADC_H*/
