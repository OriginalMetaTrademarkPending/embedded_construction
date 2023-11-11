#ifndef PWM_H
#define PWM_H
#include "sam.h"
#include "sam3x8e.h"
#include "component/component_pio.h"
#include "instance/instance_pmc.h"
#include "instance/instance_pwm.h"
#include "sam/sam3x/include/component/component_pwm.h"
/**
 * @brief Initializes the PWM module.
 */
void pwm_init();

/**
 * @brief Set the duty cycle value.
 * @param[in] value The value to be written out to the PWM channel. This is the ON period of the channel in microseconds.
*/
void pwm_set_dc(int value);

#endif /*PWM_H*/


