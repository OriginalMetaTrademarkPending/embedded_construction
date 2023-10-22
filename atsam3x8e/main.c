#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "sam.h"

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
     *  Total = 10 TQ.
     */
    const uint32_t can_br = 0x00151123;
    can_init_def_tx_rx_mb(can_br);

    CAN_MESSAGE data = {};

    while (1)
    {
        can_receive(&data, 0);
    }
    
}
