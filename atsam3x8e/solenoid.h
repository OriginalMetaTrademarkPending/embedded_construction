#ifndef SOLENOID_H
#define SOLENOID_H
#include "sam3x8e.h"
#include "component/component_dacc.h"
#include "instance/instance_dacc.h"
#include <stdint.h>
#include "timer.h"

/* Global variable for tracking the solenoid state.*/
static int solenoid_running = 0;

/**
 * @brief Initializes the solenoid pin.
*/
void solenoid_init(void);

/**
 * @brief Runs the activation logic for the solenoid when the button is pressed.
 * @param[in] button_pressed The value of the button that was pressed.
*/
void solenoid_run_button(int button_pressed);

#endif /*SOLENOID_H*/