#ifndef DAC_H
#define DAC_H
#include "sam3x8e.h"
#include "component/component_dacc.h"
#include "instance/instance_dacc.h"
#include <stdint.h>
#include "timer.h"

/**
 * Initializes the DAC Controller module.
 */
int dac_init(void);

/**
 * Writes to the DAC module.
*/
void dac_write(uint16_t data);

#endif /*DAC_H*/
