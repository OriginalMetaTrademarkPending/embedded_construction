#ifndef DAC_H
#define DAC_H
#include "sam3x8e.h"
#include "component/component_dacc.h"
#include "instance/instance_dacc.h"
#include <stdint.h>
#include "timer.h"

#define NOT_OE PIO_PD0
#define SEL PIO_PD2
#define DIR PIO_PD10
#define NOT_RST PIO_PD1
#define EN PIO_PD9

#define ENC_MSK 0x1fe

/**
 * Initializes the DAC Controller module.
 */
int dac_init(void);

/**
 * Writes to the DAC module.
*/
void dac_write(uint16_t data);

/**
 * Initializes the motor controller pins.
*/
void motor_driver();

/**
 * Start motor conversion.
*/
int16_t motor_read();

/**
 * Resets the motor encoder
*/
void reset_motor();

/**
 * Maps the joystick to the encoder
*/
uint16_t joy_map(uint8_t pos);

void encoder_calibrate();

#endif /*DAC_H*/
