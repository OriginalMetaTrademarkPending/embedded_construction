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
    float duty_cycle = (float)(raw_value)*((0.105-0.045)/(100))+0.045;
    // Writing the new value to the servo channel
    pwm_set_dc(&duty_cycle, channel);
}

