#ifndef DAC_H
#define DAC_H
#include "sam3x8e.h"
#include "component/component_dacc.h"
#include "instance/instance_dacc.h"
#include <stdint.h>
#include "timer.h"

/**
 * @brief Initializes the DAC Controller module.
 */
void dac_init(void);

/**
 * @brief Writes to the DAC module.
 * @param[in] data The value to be written to the DAC.
*/
void dac_write(uint16_t data);

#endif /*DAC_H*/
