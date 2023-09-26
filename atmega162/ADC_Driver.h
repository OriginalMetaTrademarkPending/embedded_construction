#pragma once
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif /*__AVR_ATmega162__*/
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "XMEM_Driver.h"
#define ADC_BASE_ADDR 0x1400
#define F_CPU 4915200
#include <util/delay.h>
/* Driver for interfacing with the ADC. */

typedef struct
{
	int8_t posX_t;
	int8_t posY_t;
}pos_t;

typedef enum {NEUTRAL=0, LEFT, RIGHT, UP, DOWN}joy_dir;

/**
 * @brief Initializes the ADC module.
 */
void ADC_init(void);

/**
 * @brief Reads a value from the ADC module.
 */
void ADC_read(uint8_t channel, uint8_t* result);

/**
 * @brief Calibrates the joystick. 
 */
void ADC_calibrate(uint8_t* calib_array);

/**
 * @brief Reads the joystick position. 
 */
pos_t pos_read(uint8_t* calib_array);

/**
 * @brief Reads the direction of the joystick. 
 */
joy_dir dir_read(uint8_t* calib_array);

/**
 * @brief Prints a character array from the joy_dir enum. 
 */
const char* joy_dir_to_string(joy_dir direction);
