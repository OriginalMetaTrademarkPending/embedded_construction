#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>

/**
 * @brief Delay function.
 * @param[in] us The delay time in microseconds.
*/
void _delay_us(uint16_t us);

/**
 * @brief Delay function.
 * @param[in] ms The delay time in milliseconds.
*/
void _delay_ms(uint16_t ms);

#endif /*TIMER_H*/