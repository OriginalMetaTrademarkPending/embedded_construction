#ifndef PWM_H
#define PWM_H
#include "sam.h"
#include "sam3x8e.h"
#include "component/component_pio.h"
#include "instance/instance_pmc.h"
#include "instance/instance_pwm.h"
#include "sam/sam3x/include/component/component_pwm.h"
/*
 * @brief Initializes the PWM module.
 */
int pwm_init();


/*
 * @brief Set the duty cycle value.
*/
void pwm_set_dc(float *duty_cycle, uint8_t channel);

#endif /*PWM_H*/


