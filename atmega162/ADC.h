#ifndef ADC_H
#define ADC_H
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif /*__AVR_ATmega162__*/
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "XMEM.h"
#include "CAN.h"
#define F_CPU 4915200
#include <util/delay.h>

/* Driver for interfacing with the ADC. */

typedef struct
{
	uint8_t posX_t;
	uint8_t posY_t;
}pos_t;

typedef enum {NEUTRAL=0, LEFT, RIGHT, UP, DOWN}joy_dir;

/**
 * @brief Initializes the ADC module.
 */
void ADC_init(void);

/**
 * @brief Reads a value from the ADC module.
 *
 * @param[in] channel The channel to perform the reading from.
 * @param[out] result 8-bit integer array holding all the results from a conversion cycle.
 */
void ADC_read(uint8_t channel, uint8_t* result);

/**
 * @brief Calibrates the joystick.
 * 
 * @param[out] calib_array An array containing calibration data.
 */
void ADC_calibrate(uint8_t* calib_array);

/**
 * @brief Reads the joystick position.
 *
 * @param[in] calib_array An array containing calibration data.
 *
 * @returns The position of the X and Y coordinates, as a struct pos_t.
 */
pos_t pos_read(uint8_t* calib_array);

/**
 * @brief Reads the direction of the joystick.
 *
 * @param[in] calib_array An array containing calibration data.
 *
 * @returns The direction of the joystick as an enum element.
 */
joy_dir dir_read(uint8_t* calib_array);

/**
 * @brief Prints a character array from the joy_dir enum.
 *
 * @param[in] direction The joystick direction to be parsed.
 *
 * @returns The joystick direction as a string for printing to the screen.
 */
const char* joy_dir_to_string(joy_dir direction);

/**
 * @brief Test program for the ADC module.
 */
void ADC_test(void);

/**
 * @brief Sends joystick data over CAN
 */
void ADC_send_data(uint8_t *adc_meas);

#endif /*ADC_H*/
