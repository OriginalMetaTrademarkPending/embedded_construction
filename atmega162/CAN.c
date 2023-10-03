#include "CAN.h"
#include "MCP2515.h"
#include "mcp2515_defs.h"
#include <stdint.h>

void CAN_init()
{
	MCP2515_init();
	MCP2515_reset();
	MCP2515_write(MCP_CANCTRL, ~CLKOUT_ENABLE);		// Clock output disabled
	MCP2515_write(MCP_CNF1, 0x00);				// Synchronization set to least time quanta TQ
	MCP2515_write(MCP_CNF2, 0x89);				// Configuration of propagation segments set to 2 time quanta TQ
	MCP2515_write(MCP_CNF3, 0x01);				// Configuration of propagation segments set to 2 time quanta TQ
	MCP2515_write(MCP_CANCTRL, MODE_LOOPBACK);
}

void CAN_write(CAN_frame *frame, uint8_t channel)
{
}

CAN_frame CAN_read()
{
}
