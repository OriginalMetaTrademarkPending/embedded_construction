#include "sam.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "component/component_can.h"
#include "sam3x8e.h"
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "pwm.h"
#include "servo.h"
#include "motor.h"
#include "adc.h"
#include "timer.h"
// #include "pid.h"
#include "solenoid.h"
#include "dac.h"
#define LED_D1 PIO_PA19

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    //PIOA->PIO_PER |= (LED_D1 | LED_D2);     // Enable the LED pins
    //PIOA->PIO_OER |= (LED_D1 | LED_D2);     // Set LED pins as outputs
    //PIOA->PIO_SODR |= (LED_D1 | LED_D2);    // Set output data registers. Not PUDR(pull-up disable register) nor ODSR(Output Data Status Register)
    configure_uart();
    //PIOA->PIO_SODR |= (LED_D1);     // LED_D1 set to high.
    //printf("D1 on");
    //PIOA->PIO_SODR |= (LED_D2);     // LED_D2 set to high.
    //printf("D2 on");
    /*
     *  CAN BR calculations.
     *
     *  BRP must be 20 (BRP+1 = 21)
     *  PROPSEG+1 must be 2 TQ (PROPSEG = 1)
     *  PS1 +1 must be 3 TQ (PS1 = 2)
     *  PS2 + 1 must be 4 TQ (PS2 = 3)
     *  SJW+1 is set to 1.
     *
     *  Total = 16 TQ.
     */
    const uint32_t can_br = 0x00292165;
    can_init_def_tx_rx_mb(can_br);

    CAN_MESSAGE data;   
    // Setting the servo position in %-s.
    servo_init();
    adc_init();
    dac_init();
    motor_init();
    uint16_t goal_count;
    bool goal = false;
    pos_t joy_stick_position;
    encoder_calibrate();
    _delay_ms(3000);
    while (1)
    {
        // P_REGULATOR();
        // reset_motor();
        // printf("%d\n\r", motor_read());
        can_receive(&data, 0);
        joy_stick_position.posX_t = data.data[0];
        joy_stick_position.posY_t = data.data[1];
        // printf("Data received: %u %u\n\r", joy_stick_position.posX_t, joy_stick_position.posY_t);
        //Write the x-value from the joystick
        servo_write(joy_stick_position.posX_t);
        uint16_t adc_val = ma_read();
        // printf("ADC value: %u\n\r", adc_val);
        if(adc_val < 3500){
            if(goal == false)
            {
                goal_count++;
                goal = true;
            }
        }
        else
        {
            goal = false;
        }

        // printf("Current Score: %u\n\r", goal_count);
    }
    
}
