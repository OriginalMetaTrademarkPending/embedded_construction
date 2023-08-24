#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define BASE_ADDR 0x1000
/**
 * This driver configures the MCU for usage of external SRAM.
 */

/**
 * @brief Initializes the external memory and configures the MCU.
 * @note Bus width is reduced in order to support the JTAG interface.
 */
void XMEM_init(void);

/**
 * @brief Writes to the external memory. 
 * @param data 8-bit data to be written to the external memory.
 * @param addr 16-bit address where the data will be written.
 * @param base_addr Base address for the read.
 *
 * @note The external memory is mapped as follows:
 * 0x1000-0x13FF -> OLED
 * 0x1400-0x17FF -> ADC
 * 0x1800-0x1FFF -> SRAM
 */
void XMEM_write(uint8_t data, uint16_t addr);

/*
 * @brief Reads from the external memory.
 * @param addr 16-bit address where the data will be read from.
 * @param base_addr Base address for the write.
 *
 * @note The external memory is mapped as follows:
 * 0x1000-0x13FF -> OLED
 * 0x1400-0x17FF -> ADC
 * 0x1800-0x1FFF -> SRAM
 */
uint8_t XMEM_read(uint16_t addr);

void SRAM_test(void);
