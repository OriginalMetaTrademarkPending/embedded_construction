#ifndef PID_H
#define PID_H
#include "sam.h"
#include "sam3x8e.h"
#include "component/component_tc.h"

/**
 * Some notes on the PID driver...
 *
 * Need to use a timer interrupt...how does one even do this...
 * Using a TC module might help
 *	- First initialize the timer for the PID...*/

/**
 * @brief Initializes the PID controller by setting up the timer interrupt.
 */
int pid_init();

#endif /*PID_H*/
