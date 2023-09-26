#include "OLED_Driver.h"
#include "fonts.h"
#include <stdint.h>
#include <avr/pgmspace.h>
#include <string.h>

void write_c(char command)
{
	volatile char* oled_command_mem = (char*)0x1000;
	*oled_command_mem = command;
}

void write_d(char data)
{
	volatile char* oled_data_mem = (char*)0x1200;
	*oled_data_mem = data;
}

void OLED_init()
{
	write_c(0xAE);
	write_c(0xA1);
	write_c(0xDA);
	write_c(0x12);
	write_c(0xC8);
	write_c(0xA8);
	write_c(0x3F);
	write_c(0xD5);
	write_c(0x80);
	write_c(0x81);
	write_c(0x50);
	write_c(0xD9);
	write_c(0x21);
	write_c(0x20);
	write_c(0x02);
	write_c(0xDB);
	write_c(0x30);
	write_c(0xAD);
	write_c(0x00);
	write_c(0xA4);
	write_c(0xA6);
	write_c(0xAF);
}

void OLED_goto_line(uint8_t line)
{
	write_c(0xB0 + line);
}

void OLED_goto_column(uint8_t column)
{
	write_c(0x00 + (column%16));
	write_c(0x10 + (column/16));
}

void OLED_goto_pos(uint8_t line, uint8_t column)
{
	OLED_goto_line(line);
	OLED_goto_column(column);
}

void OLED_clear()
{
	write_c(0xAE);
	for(uint8_t page = 0; page < 8; page++)
	{
		OLED_goto_line(page);
		for(uint8_t segment = 0; segment < 128; segment++)
		{
			OLED_goto_column(segment);
			write_d(0x00);
		}
	}
	write_c(0xAF);
}

int OLED_printf_char(char ch)
{
	ch -= 32;
	for(int i = 0; i < 5; i++)
	{
		int byte = pgm_read_byte(&font5[ch][i]);
		write_d(byte);
	}
	return 0;
}

int OLED_printf(char *string)
{
	for(uint8_t i = 0; i < strlen(string); i++)
	{
		OLED_printf_char(string[i]);
	}
	return 0;
}

void OLED_set_brightness(uint8_t level)
{
	write_c(0x81);
	write_c(level);
}

void OLED_clear_line(uint8_t line)
{
	OLED_goto_line(line);
	for(uint8_t col = 0; col < 128; col++)
	{
		OLED_goto_column(col);
		write_d(0x00);
	}
}

void OLED_clear_columns(uint8_t start, uint8_t end)
{
	for (uint8_t row = 0; row < 8; row++){
		for(uint8_t col = start; col <= end; col++)
		{
			OLED_goto_pos(row, col);
			write_d(0x00);
		}
	}
}

void OLED_home(int state)
{
	OLED_goto_pos(1, 32);
	OLED_printf("PING PONG");
	OLED_goto_pos(3, 10);
	OLED_printf("Start");
	OLED_goto_pos(4, 10);
	OLED_printf("Highscores");
	OLED_goto_pos(5, 10);
	OLED_printf("Settings");
	OLED_goto_pos(6, 10);
	OLED_printf("Quit");
}
