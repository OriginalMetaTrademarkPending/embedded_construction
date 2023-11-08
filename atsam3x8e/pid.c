#include "pid.h"

int pid_init(uint32_t freq)
{
	// Initializing the PI regulator.
	PID_regulator.Kp = 0.1;
	PID_regulator.Ki = 0.5;
	PID_regulator.T = 1.0/freq;
	PID_regulator.error_sum = 0;
	// Disabling the 
	NVIC_DisableIRQ(TC0_IRQn);
	// First, configuring the PMC to enable the TC clock
	// Now, enabling the peripheral clock.
	PMC->PMC_PCER0 |= 0x1u << ID_TC0;
	// Configuring the waveform mode
	REG_TC0_CMR0 = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET;
	// Generating a 50% duty cycle waveform.
	TC0->TC_CHANNEL[0].TC_RC = (1.0/freq)*2625;
	// Enable the interrupt routine for RC compare.
	TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
	// Finally, connect the nested interrupt controller.
	NVIC_EnableIRQ(TC0_IRQn);
	// Here the TC module must be started.
	REG_TC0_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

void TC0_Handler()
{
	//When the interrupt is called, 1 cycle of the PI controller must be executed.
	CAN_MESSAGE msg;
	can_receive(&msg, 0);
	int16_t ref_pos = joy_map(msg.data[1]);
	int16_t current_pos = motor_read();
	int16_t error = ref_pos - current_pos;
	PID_regulator.error_sum += error;
	int16_t u = (PID_regulator.Kp*error) + (PID_regulator.Ki*PID_regulator.T*PID_regulator.error_sum);
	if(error < 0)
	{
		PIOD->PIO_CODR = DIR;
	}
	else
	{
		PIOD->PIO_SODR = DIR;
	}
	dac_write(abs(u));
	uint32_t status = TC0->TC_CHANNEL[0].TC_SR;
	// The value from this function must be written to the motor.
}


void P_REGULATOR()
{
	int16_t K_p = 1;
	CAN_MESSAGE msg;
	can_receive(&msg, 0);
	int16_t ref_pos = joy_map(msg.data[1]);
	int16_t current_pos = motor_read();
	int16_t error = ref_pos - current_pos;
	int16_t u = K_p * error;
	int MagicNumber = 40;
	int speed = abs(u)/MagicNumber;
	if(error > 0)
	{
		PIOD->PIO_CODR = DIR;
	}
	else
	{
		PIOD->PIO_SODR = DIR;
	}
	dac_write(speed);

}