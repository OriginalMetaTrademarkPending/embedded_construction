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


/**
 * Some notes on the PID driver...
 *
 * Need to use a timer interrupt...how does one even do this...
 * Using a TC module might help
 *	- First initialize the timer for the PID...*
 *	- Set the PMC write protection to false and enable the peripheral clk for TC0
 *	- Configure the TC module (look at the datasheet.
 *	- */

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
 * @brief Initializes the PID_regulator controller by setting up the timer interrupt.
 */
int pid_init(uint32_t freq);

/**
 * @brief TC0 interrupt handler.
 */
void TC0_Handler();

/**
 * @brief pppp.
 */
void P_REGULATOR();


#endif /*PID_H*/
