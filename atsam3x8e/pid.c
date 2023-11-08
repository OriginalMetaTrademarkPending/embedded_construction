#include "pid.h"

PID_t PID_regulator;

int pid_init(uint32_t freq)
{
	printf("oogabooga 1");
	// Initializing the PI regulator.
	PID_regulator.Kp = 0.1;
	PID_regulator.Ki = 10.0;
	PID_regulator.Kd = 0.2;
	PID_regulator.T = 1.0/freq;
	PID_regulator.err[0] = 0;
	PID_regulator.err[1] = 0;
	PID_regulator.error_sum = PID_regulator.err[1];
	// Disabling the 
	NVIC_DisableIRQ(TC0_IRQn);
	// First, configuring the PMC to enable the TC clock
	// Now, enabling the peripheral clock.
	PMC->PMC_PCER0 |= 0x1u << ID_TC0;
	// Configuring the waveform mode
	REG_TC0_CMR0 = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET;
	// Generating a 50% duty cycle waveform.
	TC0->TC_CHANNEL[0].TC_RC = (uint32_t)(CHIP_FREQ_CPU_MAX/512/freq);
	// Enable the interrupt routine for RC compare.
	TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
	// Finally, connect the nested interrupt controller.
	NVIC_EnableIRQ(TC0_IRQn);
	// Here the TC module must be started.
	REG_TC0_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;
	printf("oogabooga 1");
}

void TC0_Handler()
{
	printf("oogabooga2");
	//When the interrupt is called, 1 cycle of the PI controller must be executed.
	CAN_MESSAGE msg;
	can_receive(&msg, 0);
	int16_t ref_pos = joy_map(msg.data[1]);
	int16_t current_pos = motor_read();
	PID_regulator.err[1] = PID_regulator.err[0];
	PID_regulator.err[0] = ref_pos - current_pos;
	PID_regulator.error_sum += PID_regulator.err[0];
	int16_t err_deriv = PID_regulator.err[0] - PID_regulator.err[1];
	int16_t u = (int16_t)((PID_regulator.Kp*PID_regulator.err[0]) + (PID_regulator.Ki*PID_regulator.T*PID_regulator.error_sum) + (PID_regulator.Kd/PID_regulator.T*(err_deriv)));
	if(PID_regulator.err[0] > 0)
	{
		PIOD->PIO_CODR = DIR;
	}
	else
	{
		PIOD->PIO_SODR = DIR;
	}
	dac_write(abs(u)/70);
	printf("Written to motor: %d\n\r", abs(u));
	servo_write(msg.data[0]);
	uint16_t status = TC0->TC_CHANNEL[0].TC_SR;
	NVIC_ClearPendingIRQ(ID_TC0);
	// The value from this function must be written to the motor.
	printf("oogabooga2");
}


void P_REGULATOR()
{
	int16_t K_p = 1;
	CAN_MESSAGE msg;
	can_receive(&msg, 0);
	int16_t ref_pos = joy_map(msg.data[0]);
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