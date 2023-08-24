#include "UART_Driver.h"
#include <stdint.h>

/**
 * VALIDATED!
 */
void USART_Init(uint64_t ubrr)
{
	/* Set baud rate for the USART module.
	*  We start by setting the high bits, then the low bits. */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)(ubrr);
	/* Activate both receive and transmit functionality. */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Configuring the USART module.
	*  We want 2 stop bits and a data format of 8 bits per package. */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}


/**
 * VALIDATED!
 */
int USART_Transmit_single(char ascii_char, FILE* stream)
{
	/* Wait for the transmit buffer to be empty.
	*  If UDRE0 returns a 1, it means the data register is empty.
	*  At that point, transmit the character. 
	*/
	while(!(UCSR0A&(1<<UDRE0)));
	UDR0 = ascii_char;
	if(TXC0)
	{
		return 0;
	}
	return -1;
}

/**
 * VALIDATED!
 */
int USART_Receive_single(FILE* stream)
{
	/* Wait for data to be received.
	*  If RXC0 returns a 1, 
	*  it means the receive operation has been completed.
	*  Return the data written to UDR0.
	*/
	while(!(UCSR0A&(1<<RXC0)));
	if(RXC0)
	{
		if (stream != NULL)
		{
			return UDR0;
		}
		else
		{
			return _FDEV_EOF;
		}
	}
	else
	{
		return _FDEV_ERR;
	}
}

/**
 * VALIDATED!
 */
FILE* USART_stream_setup(void)
{
	return fdevopen(USART_Transmit_single, USART_Receive_single);
}
