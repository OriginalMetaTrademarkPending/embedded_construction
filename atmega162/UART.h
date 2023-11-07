#ifndef UART_H
#define UART_H
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif /*__AVR_ATmega162__*/
#include <avr/io.h>
#include "avr/interrupt.h"
#include <stdint.h>
#include <stdio.h>

#define UART_RINGBUFFER_SIZE 64

/**
 * @brief Struct for ringbuffer implementation of the UART driver.
 */
typedef struct
{
	uint8_t head, tail;
	char data[UART_RINGBUFFER_SIZE]; 
}uart_ringbuffer_t;

/**
 * @brief Interrupt service routines for the UART module.
 */
ISR(USART0_RXC);

/**
* @brief Initiates the USART module.
* @param ubrr The USART baud rate.
*
* @note UBRR is calculated as ((FOSC/(16-BAUDRATE))-1).
*/
void UART_Init(uint64_t ubrr);

/**
 * @brief Transmits a single ASCII character through the USART line.
 * @param ascii_char The ASCII character to be transmitted.
 *
 * @return The value of TXC0 (USART0 Transfer Complete bit).
 *
*/
int UART_Transmit_single(char ascii_char, FILE* stream);

/**
 * @brief Receives a single ASCII character from the USART line.
 * @return The received character from the USART line as an unsigned 8-bit integer.
*/
int UART_Receive_single(FILE* stream);

/**
 * @brief Wrapper function for printf. Connects printf to the MCU.
 * @param format A const formatted string (check printf for details).
 * 
 * @return 0 if printf was successful, some value if it wasn't.
*/
FILE* UART_stream_setup(void);

/**
 * @brief Test program for the USART module.
 */
void UART_test(uint64_t ubrr);
#endif /*UART_H*/
