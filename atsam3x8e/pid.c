#include "pid.h"


PID_t PID_regulator;
goal_params Goal_structure;
CAN_MESSAGE Outgoing_message;


void pid_init(uint32_t freq)
{
	// Initializing the PI regulator.
	PID_regulator.Kp = 0.1;
	PID_regulator.Ki = 0.2;
	PID_regulator.Kd = 0;
	PID_regulator.T = 1.0/freq;
	PID_regulator.err[0] = 0;
	PID_regulator.err[1] = 0;
	PID_regulator.error_sum = PID_regulator.err[1];
	// Disabling the nested vector interrupt for the timer counter.
	NVIC_DisableIRQ(TC0_IRQn);
	// First, configuring the PMC to enable the TC clock
	// Now, enabling the peripheral clock.
	PMC->PMC_PCER0 |= 0x1u << ID_TC0;
	// Configuring the waveform mode
	REG_TC0_CMR0 = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET;
	// Generating a 50% duty cycle waveform.
	TC0->TC_CHANNEL[0].TC_RC = (uint32_t)(CHIP_FREQ_CPU_MAX/128/freq);
	// Enable the interrupt routine for RC compare.
	TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
	// Finally, connect the nested interrupt controller.
	NVIC_EnableIRQ(TC0_IRQn);
	// Here the TC module must be started.
	REG_TC0_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;
	Outgoing_message.id = 0x0020;
	Outgoing_message.data_length = 1;
}

void TC0_Handler()
{
	CAN_MESSAGE	Incomming_message;
	//When the interrupt is called, 1 cycle of the PI controller must be executed.
	can_receive(&Incomming_message, 0);
	int16_t ref_pos = joy_map(Incomming_message.data[1]);
	int16_t current_pos = motor_read();
	uint16_t IR_sensor_val = ma_read();
	printf("Value read from ma_read: %u \n\r", IR_sensor_val);
	bool sensor_passed = false;

	PID_regulator.err[1] = PID_regulator.err[0];
	PID_regulator.err[0] = ref_pos - current_pos;
	PID_regulator.error_sum += PID_regulator.err[0];
	int16_t err_deriv = PID_regulator.err[0] - PID_regulator.err[1];
	int16_t u = (int16_t)((PID_regulator.Kp*PID_regulator.err[0]) + (PID_regulator.Ki*PID_regulator.T*PID_regulator.error_sum)+ (PID_regulator.Kd/PID_regulator.T*(err_deriv)));
	solenoid_run_button(Incomming_message.data[4]);
	if(abs(u) < 0.5)
	{
		PIOD->PIO_CODR = EN;
	}
	else{
		PIOD->PIO_SODR = EN;
	}
	if(PID_regulator.err[0] > 0)
	{
		PIOD->PIO_CODR = DIR;
	}
	else
	{
		PIOD->PIO_SODR = DIR;
	}
	dac_write(abs(u)*1000);
	//printf("Written to motor: %d\n\r", abs(u));
	servo_write(Incomming_message.data[3]);


	// If the sensor has been crossed, increment the goal counter
    if(!Goal_structure.goal && IR_sensor_val < 3900)
    {
        Goal_structure.goal = 1;
		if (!Goal_structure.goal)
		{
			Goal_structure.goal = 1;
		}
		Goal_structure.goal_count++;
    }
    else if (!(IR_sensor_val < 3900))
    {
        Goal_structure.goal = 0;
    }
	
	printf("%d \n\r", Goal_structure.goal_count);
	Outgoing_message.data[0] = Goal_structure.goal_count;
	can_send(&Outgoing_message, 1);
	uint16_t status = TC0->TC_CHANNEL[0].TC_SR;
	NVIC_ClearPendingIRQ(ID_TC0);
}