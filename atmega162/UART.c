#include "UART.h"

/* Static ringbuffer for receive functionality. */
uart_ringbuffer_t rx_buffer;

/**
 * VALIDATED!
 */
ISR(USART0_RXC_vect)
{
	/* First, check for overflow error or frame error. */
	if(UCSR0A & ((1<<FE0)|(1<<DOR0)));
	else
	{
		/* Check if receive ringbuffer is full. */
		if((rx_buffer.tail + 1) % UART_RINGBUFFER_SIZE == rx_buffer.head)
		{
			printf("ERR: UART RX buffer is full\n\r");
			rx_buffer.data[rx_buffer.tail] = UDR0;
		}
		else
		{
			rx_buffer.data[rx_buffer.tail] = UDR0;
			rx_buffer.tail = (rx_buffer.tail + 1) % UART_RINGBUFFER_SIZE;
		}
	}
}

/**
 * VALIDATED!
 */
void UART_Init(uint64_t ubrr)
{
	/* First, disable all interrupts.*/
	cli();
    /* Initialize rx_buffer*/
    rx_buffer.head = 0;
    rx_buffer.tail = 0;
	/* Set baud rate for the USART module.
	*  We start by setting the high bits, then the low bits. */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)(ubrr);
	/* Activate both receive and transmit functionality, as well as the receive interrupt. */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/* Configuring the USART module.
	*  We want 2 stop bits and a data format of 8 bits per package. */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
	/* Connect the USART interface to the standard IO streams. */
	UART_stream_setup();
	/* Activate all interrupts. */
	sei();
}


/**
 * VALIDATED!
 */
int UART_Transmit_single(char ascii_char, FILE* stream)
{
	/* Wait for the transmit buffer to be empty.
	*  If UDRE0 returns a 1, it means the data register is empty.
	*  At that point, transmit the character. 
	*/
	while(!(UCSR0A&(1<<UDRE0)));
	UDR0 = ascii_char;
	return 0;
}

/**
 * VALIDATED!
 */
int UART_Receive_single(FILE* stream)
{
	/* Check if a character is available in the ringbuffer. */
	if(rx_buffer.head == rx_buffer.tail)	// Buffer is empty
	{
		return 1;
	}
	rx_buffer.head = (rx_buffer.head + 1) % UART_RINGBUFFER_SIZE;
	return (rx_buffer.data[rx_buffer.head - 1]);
	/* Read the head character from the ringbuffer. */

}

/**
 * VALIDATED!
 */
FILE* UART_stream_setup(void)
{
	return fdevopen(UART_Transmit_single, UART_Receive_single);
}

void UART_test(uint64_t ubrr)
{
	UART_Init(ubrr);
	char test_data;
	while (1)
	{
		scanf("%c", &test_data);
		printf("Retrieved data: %c\n\r", test_data);
	}
}
