#pragma once
#include <avr/io.h>
/* Driver for interfacing with the ADC. */

/**
 * @brief Initializes the ADC module.
 */
void ADC_init(void);

/**
 * @brief Test function to check whether the ADC is getting selected. 
 */
void ADC_test(void);
