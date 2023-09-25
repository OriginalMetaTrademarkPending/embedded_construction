#pragma once
#include <avr/io.h>
#include <stdint.h>
/* Driver for interfacing with the ADC. */

/**
 * @brief Struct converting the raw joystick data into percentages.
 */
typedef struct
{
	int8_t posX_t;
	int8_t posY_t;
}pos_t;

/**
 * @brief Joystick direction enum. 
 */
typedef enum {NEUTRAL=0, LEFT, RIGHT, UP, DOWN}joy_dir;

/**
 * @brief Initializes the ADC module.
 */
void ADC_init(void);

/**
 * @brief Reads a value from the ADC module.
 *
 * @param[in] channel The ADC channel to read from.
 * @param[out] result The output buffer where the ADC will write its measurements.
 */
void ADC_read(uint8_t channel, uint8_t* result);

/**
 * @brief Calibrates the joystick.
 *
 * @param[out] calib_array A buffer with calibration data written by the calibration function.
 */
void ADC_calibrate(uint8_t* calib_array);

/**
 * @brief Reads the joystick position and converts the raw values to percentages.
 *
 * @param[in] calib_array A buffer with calibration data used to convert the raw data to percentages.
 * @return A struct holding the joystick position in both the x and y axes in percentages.
 */
pos_t pos_read(uint8_t* calib_array);

/**
 * @brief Reads the direction of the joystick based on its position.
 *
 * @param[in] A struct containing the joystick position.
 * @return The direction of the joystick.
 */
joy_dir dir_read(pos_t* position);

/**
 * @brief Prints a character array from the joy_dir enum.
 *
 * @param[in] The direction of the joystick as an enum.
 * @return A string literal based on the direction.
 */
const char* joy_dir_to_string(joy_dir direction);
