#include "solenoid.h"


void solenoid_init()
{
    PIOA->PIO_PER |= PIO_PA24;
    PIOA->PIO_OER |= PIO_PA24;
    PIOA->PIO_SODR |= PIO_PA24;
}

void solenoid_run_button(int button_pressed)
{
    if(!solenoid_running && button_pressed)
    {
        solenoid_running = 1;

        PIOA->PIO_SODR |= PIO_PA24;
        _delay_ms(100);
        PIOA->PIO_CODR |= PIO_PA24;
        _delay_ms(100);
    }

    else if (!button_pressed)
    {
        solenoid_running = 0;
    }
}