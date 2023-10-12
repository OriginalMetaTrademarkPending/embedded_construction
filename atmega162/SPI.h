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
 * @brief Performs a register shift on the MISO/MOSI line.
 *
 * @param[in] The byte to be shifted to the slave.
 */
void SPI_transmit(uint8_t data);

/**
 * @brief Receives data from the slave.
 *
 * @return The byte shifted by the slave
 */
uint8_t SPI_receive(void);
#endif /*SPI_H*/
