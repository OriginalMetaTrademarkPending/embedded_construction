#ifndef PID_H
#define PID_H
#include "sam.h"
#include "sam3x8e.h"
#include "component/component_tc.h"
#include "dac.h"
#include "servo.h"
#include "instance/instance_tc0.h"
#include <stdint.h>
#include "can_controller.h"
#include "motor.h"
#include "solenoid.h"
#include "adc.h"

/**
 * @brief Struct containing the PID configuration. It has been initialized as a static variable.
 */
typedef struct
{
	float Kp;
	float Ki;
	float Kd;
	float T;
	int16_t error_sum;
	int16_t err[2];
}PID_t;

/**
 * @brief Struct to track number of goals and if a goal was scored in the previous timestep.
*/
typedef struct
{
	uint8_t goal;	
	uint16_t goal_count;

}goal_params;

/**
 * @brief Initializes the PID_regulator controller by setting up the timer interrupt.
 * @param[in] freq The sampling frequency of the PID controller.
 */
void pid_init(uint32_t freq);

/**
 * @brief TC0 interrupt handler.
 */
void TC0_Handler();


#endif /*PID_H*/
