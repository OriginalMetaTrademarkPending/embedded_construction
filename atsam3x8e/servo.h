#ifndef SERVO_H
#define SERVO_H
#include "sam.h"
#include "pwm.h"

// Define the pos_t structure such that the x- and y-values can be extracted.
typedef struct
{
	int8_t posX_t;
	int8_t posY_t;
}pos_t;


/*
 * @brief Initializes the servo.
 */
void servo_init();

/*
 * @brief Write the servo value. Converting the raw value to a duty cycle-value. [0-100]
 */
void servo_write(int raw_value);


#endif /*SERVO_H*/