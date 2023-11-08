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
 * @brief Interrupt service routine for the UART module.
 */
ISR(USART0_RXC_vect);

/**
* @brief Initiates the UART module.
* @param ubrr The UART baud rate.
*
* @note UBRR is calculated as ((FOSC/(16-BAUDRATE))-1).
*/
void UART_Init(uint64_t ubrr);

/**
 * @brief Transmits a single ASCII character through the UART line via polling.
 * @param ascii_char The ASCII character to be transmitted.
 *
 * @return 0 if transmission succeeded.
 *
*/
int UART_Transmit_single(char ascii_char, FILE* stream);

/**
 * @brief Receives a single ASCII character from the UART ringbuffer.
 * @return The head character from the UART ringbuffer.
*/
int UART_Receive_single(FILE* stream);

/**
 * @brief Wrapper function for printf. Connects printf to the MCU.
 * 
 * @return The file handle for the stream.
*/
FILE* UART_stream_setup(void);

/**
 * @brief Test program for the USART module.
 * @param ubrr The baud rate for the communication.
 */
void UART_test(uint64_t ubrr);
#endif /*UART_H*/
