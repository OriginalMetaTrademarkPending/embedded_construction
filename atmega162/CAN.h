#ifndef CAN_H
#define CAN_H
#include <stdint.h>
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif /*__AVR_ATmega162__*/
#include "MCP2515.h"
/* High level driver for sending and receiving CAN messages. */

/**
 * @brief Dataframe for the CAN driver.
 */
typedef struct
{
	uint16_t id;			// ID of the CAN dataframe
	uint8_t frame_length;		// Dataframe length
	char data[8];			// Dataframe content
}CAN_frame;

/*
 * @brief Initializes the CAN module.
 */
void CAN_init(uint8_t mode);

/**
 * @brief Sends a dataframe over the CAN network.
 */
void CAN_write(CAN_frame* frame);

/**
 * @brief Receives a dataframe from the CAN network.
 */
CAN_frame CAN_read();

/**
 * @brief Tests the CAN network.
 *
 * @warning This test does not initialize the UART interface. You must call USART_Init() separately in the main.c file.
 */
void CAN_test();

#endif /*CAN_H*/
