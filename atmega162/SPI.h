#ifndef SPI_H
#define SPI_H
#include <stdint.h>
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif /*__AVR_ATmega162__*/
#include <avr/io.h>
#define SS PB4
#define MOSI PB5
#define MISO PB6
#define SCK PB7
/* SPI communication driver. */

/**
 * @brief Initializes the SPI interface.
 */
void SPI_init();

/**
 * @brief Sends a byte to the specified slave unit.
 *
 * @param[in] The byte to be sent.
 */
void SPI_send(uint8_t data);

/**
 * @brief Reads a byte from the specified slave unit.
 * 
 * @return The byte that was read.
 */
uint8_t SPI_read();
#endif /*SPI_H*/
