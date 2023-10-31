#include "pid.h"

int pid_init()
{
	// Here the TC module must be started.
	REG_TC0_CCR0 = TC_CCR_CLKEN;
}
