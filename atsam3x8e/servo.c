#include "servo.h"


void servo_init()
{
    // Initialize the pwm
    pwm_init();
    // Set the servo to the zero-position.
    servo_write(0);
}

void servo_write(uint8_t raw_value)
{
    // Channel 5 is used for the servo.
    int channel = 5;
    // Converting to new range
    // New_value = (old_value-min_old)* ((max_new-min_new)/(max_old-min_old)) + min_new
    int duty_cycle = (int)(raw_value)*((2100.0-900.0)/(100.0))+ 900;
    // Writing the new value to the servo channel
    pwm_set_dc(duty_cycle);
}

