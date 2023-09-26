#include "UART.h"

ISR(USART0_RXC_vect)
{
	PORTB |= UDR0;
}

ISR(USART0_UDRE_vect)
{
	UDR0 = PORTB;
	UCSR0B &= ~(1<<UDRIE0);
}

/**
 * VALIDATED!
 */
void USART_Init(uint64_t ubrr)
{
	/* Set baud rate for the USART module.
	*  We start by setting the high bits, then the low bits. */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)(ubrr);
	/* Activate both receive and transmit functionality. Additionally, enable the interrupt bit for the RXC flag. */
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
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
	PORTB = ascii_char;
	UCSR0B |= (1<<UDRIE0);
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
	return PORTB;
}

/**
 * VALIDATED!
 */
FILE* USART_stream_setup(void)
{
	/* Return the fdevopen link to printf. */
	return fdevopen(USART_Transmit_single, USART_Receive_single);
}
