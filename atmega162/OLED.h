#ifndef OLED_H
#define OLED_H
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif /*__AVR_ATmega162__*/
#include <stdint.h>
#include <string.h>
#include "fonts.h"
#include "ADC.h"
#ifndef F_CPU
#define F_CPU 4915200
#endif /*F_CPU*/
#include <util/delay.h>
/* Driver for the OLED screen.*/

/**
 * @brief Helper function to write commands to the OLED driver.
 */
void write_c(char command);

/**
 * @brief Helper function to write data to the OLED GDDRAM.
 */
void write_d(char data);

/**
 * @brief Initializes the OLED Driver.
 */
void OLED_init();

/**
 * @brief Moves the cursor to the specified line/page.
 */
void OLED_goto_line(uint8_t line);

/**
 * @brief Moves the cursor to the specified column/segment.
 */
void OLED_goto_column(uint8_t column);

/**
 * @brief Moves the cursor to the specified position.
 */
void OLED_goto_pos(uint8_t line, uint8_t column);

/**
 * @brief Clears the OLED screen.
 */
void OLED_clear();

/**
 * @brief Prints a character in the OLED screen.
 */
int OLED_printf_char(char ch);

/**
 * @brief Prints a string in the OLED screen.
 */
int OLED_printf(char* string);

/**
 * @brief Sets the contrast of the OLED screen.
 */
void OLED_set_brightness(uint8_t level);

/**
 * @brief Clears an entire row/page.
 */
void OLED_clear_line(uint8_t line);

/**
 * @brief Clears everything between two segments/columns.
 */
void OLED_clear_columns(uint8_t start, uint8_t end);

/**
 * @brief The home menu for the game.
 */
void OLED_home_screen(int state);

/**
 * @brief The settings menu for the game.
 */
void OLED_settings(int state);

/**
 * @brief The highscores menu for the game.
 */
void OLED_highscores_screen(uint8_t* scores);

/**
 * @brief The screen for when the joystick is calibrating.
 */
void OLED_calibrate_screen();

/**
 * @brief The screen for when the game is running.
 */
void OLED_game_running();

/**
 * @brief Tests the functionality of the OLED screen.
*/
void OLED_test();
#endif /*OLED_H*/
