#ifndef MCP2515_H
#define MCP2515_H
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif /*__AVR_ATmega162__*/
#include <stdint.h>
#include <stdio.h>
#include "mcp2515_defs.h"
#include "SPI.h"
#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 4915200
#endif /*F_CPU*/
#include <util/delay.h>
/* Low level driver for setting up the CAN controller and accessing its control and status registers. */

/**
 * @brief Resets the MCP2515 module.
 */
void MCP2515_reset();

/**
 * @brief Initializes the MCP2515 CAN module.
 * @return -1 in case of failed initialization, 0 on success.
 */
int MCP2515_init();

/**
 * @brief Reads a CAN message from the bus.
 *
 * @param[in] address The address to read from.
 * @param[out] data The data buffer read from the SPI line.
 */
uint8_t MCP2515_read(uint8_t address);

/**
 * @brief Writes a CAN message from the bus.
 *
 * @param[in] address The address to write to.
 * @param[in] data The data to be written to the SPI line.
 */
void MCP2515_write(uint8_t address, char data);

/**
 * @brief Sends a request to transmit a CAN message.
 *
 * @param[in] channel The transmission channel to send the request.
 */
void MCP2515_request_to_send(uint8_t tx_buffer);

/**
 * @brief Modifies specific bits in a bitfield of the MCP2515.
 *
 * @param[in] address The address to modify the bits in.
 * @param[in] mask_byte The bit mask to modify.
 * @param[in] data The data to be modified.
 */
void MCP2515_bit_modify(uint8_t address, uint8_t mask_byte, char data);

/**
 * @brief Reads the status of the MCP2515 module.
 *
 * @param[out] status The buffer to write the status of the MCP2515 module to.
 */
uint8_t MCP2515_read_status();
#endif /*MCP2515_H*/
