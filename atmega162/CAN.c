#include "CAN.h"
#include "MCP2515.h"
#include "mcp2515_defs.h"
#include <stdint.h>

void CAN_init()
{
	MCP2515_init();
	MCP2515_reset();
	MCP2515_write(MCP_CANINTE, MCP_NO_INT);			// Interrupts disabled
	MCP2515_write(MCP_CNF1, 0x3F);
	MCP2515_write(MCP_CANCTRL, MODE_LOOPBACK);
}

void CAN_write(CAN_frame *frame)
{
	// CAN sending logic is as follows...
	// Sequence begins when the device determines that the TXBnCTRL.TXREQ for any of the transmit registers has been set.
	// For now, assume the CAN bus is available to start transmission. Set the id of the message.
	MCP2515_write(MCP_TXB0SIDH, ((frame->id) >> 8));
	MCP2515_write(MCP_TXB0SIDL, (frame->id));
	
	// Set the data length of the message.
	MCP2515_write(MCP_TXB0DLC, (frame->frame_length));

	// Set the data bytes to be transmitted.
	for(uint8_t _byte = 0; _byte < (frame->frame_length); _byte++)
	{
		MCP2515_write(MCP_TXB0D0 + _byte, (frame->data)[_byte]);
	}

	MCP2515_request_to_send(MCP_RTS_TX0);
}

CAN_frame CAN_read()
{
	CAN_frame result;
	uint8_t id_low;
	uint8_t id_high;
	id_low = MCP2515_read(MCP_RXB0SIDL)/0b100000;
	id_high = MCP2515_read(MCP_RXB0SIDH);
	result.id = (id_high*0b1000) + id_low;
	result.frame_length = MCP2515_read(MCP_RXB0DLC);
	for(uint8_t i = 0; i < result.frame_length; i++)
	{
		result.data[i] = MCP2515_read(MCP_RXB0D0 + i);
	}
	return result;
}
