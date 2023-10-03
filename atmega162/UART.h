#ifndef UART_H
#define UART_H
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif /*__AVR_ATmega162__*/
#include <avr/io.h>
#include "avr/interrupt.h"
#include <stdint.h>
#include <stdio.h>

/**
* @brief Initiates the USART module.
* @param ubrr The USART baud rate.
*
* @note UBRR is calculated as ((FOSC/(16-BAUDRATE))-1).
*/
void USART_Init(uint64_t ubrr);

/**
 * @brief Transmits a single ASCII character through the USART line.
 * @param ascii_char The ASCII character to be transmitted.
 *
 * @return The value of TXC0 (USART0 Transfer Complete bit).
 *
*/
int USART_Transmit_single(char ascii_char, FILE* stream);

/**
 * @brief Receives a single ASCII character from the USART line.
 * @return The received character from the USART line as an unsigned 8-bit integer.
*/
int USART_Receive_single(FILE* stream);

/**
 * @brief Wrapper function for printf. Connects printf to the MCU.
 * @param format A const formatted string (check printf for details).
 * 
 * @return 0 if printf was successful, some value if it wasn't.
*/
FILE* USART_stream_setup(void);
#endif /*UART_H*/
