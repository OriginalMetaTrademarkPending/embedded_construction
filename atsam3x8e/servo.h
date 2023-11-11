#ifndef SERVO_H
#define SERVO_H
#include "sam.h"
#include "pwm.h"

/**
 * @brief Struct to hold the position of the joystick received from node 2.
*/
typedef struct
{
	uint8_t posX_t;
	uint8_t posY_t;
}pos_t;


/**
 * @brief Initializes the servo.
 */
void servo_init();

/**
 * @brief Write the servo value.
 * @param[in] raw_value The value to be converted from the DAC module.
 */
void servo_write(uint8_t raw_value);


#endif /*SERVO_H*/
