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
#include "pid.h"
#include "dac.h"
#include "core_cm3.h"

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    configure_uart();
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
    NVIC_SetPriority(CAN0_IRQn, 1);
    NVIC_SetPriority(TC0_IRQn, 2);
    pwm_init();
    servo_init();
    adc_init();
    dac_init();
    motor_init();
    solenoid_init();
    encoder_calibrate();
    uint16_t goal_count;
    bool goal = false;
    pos_t joy_stick_position;
    pid_init(5000);
    __enable_irq();
    while (1);
}
