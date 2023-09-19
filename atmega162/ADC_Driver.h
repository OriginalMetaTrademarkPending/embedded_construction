#pragma once
#include <avr/io.h>
#include <stdint.h>
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
