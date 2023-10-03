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
	uint8_t* data;			// Dataframe content
}CAN_frame;

/*
 * @brief Initializes the CAN module.
 */
void CAN_init();

/**
 * @brief Sends a dataframe over the CAN network.
 */
void CAN_write(CAN_frame* frame, uint8_t channel);

/**
 * @brief Receives a dataframe from the CAN network.
 */
CAN_frame CAN_read();
/**
 * @brief */

#endif /*CAN_H*/
