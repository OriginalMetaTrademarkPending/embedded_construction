#ifndef MOTOR_H
#define MOTOR_H
#include "dac.h"
// PINS FOR MJ1 og MJ2

/* !OE = PIN25 = PD0
 * SEL = PIN27 = PD2
 * DIR = PIN32 = PD10
 * NOT_RST = PIN26 = PD1
 * EN = PIN30 = PD9
 */

/* D00 = PIN33 = PC1
 * D01 = PIN34 = PC2
 * DO2 = PIN35 = PC3
 * DO3 = PIN36 = PC4
 * DO4 = PIN37 = PC5
 * DO5 = PIN38 = PC6
 * DO6 = PIN39 = PC7
 * DO7 = PIN40 = PC8
 */
#define NOT_OE PIO_PD0
#define SEL PIO_PD2
#define DIR PIO_PD10
#define NOT_RST PIO_PD1
#define EN PIO_PD9

#define ENC_MAX 1400
#define ENC_MSK 0x1fe
#define ENC_MIN 0
#define ENC_MID ENC_MAX/2

extern int16_t max_right;
extern int16_t max_left;

/**
 * @brief Initializes the motor driver
*/
void motor_init();

/**
 * @brief Start motor conversion.
 * @returns The value read by the encoder.
*/
int16_t motor_read();

/**
 * @brief Resets the motor encoder
*/
void reset_motor();

/**
 * @brief Runs a calibration of the encoder
*/
void encoder_calibrate();

/**
 * @brief Maps the joystick values to the encoder values for the motor.
 *
 * @note This function is necessary in order to map the joystick values to the encoder values.
*/
uint16_t joy_map(uint8_t pos);

#endif /*MOTOR_H*/