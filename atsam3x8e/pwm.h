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
 * @brief Set the DC value between 0,9-2,1V.
*/
void pwm_set_dc(int value);

#endif /*PWM_H*/


