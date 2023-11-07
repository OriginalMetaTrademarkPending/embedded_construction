#ifndef SOLENOID_H
#define SOLENOID_H
#include "sam3x8e.h"
#include "component/component_dacc.h"
#include "instance/instance_dacc.h"
#include <stdint.h>
#include "timer.h"

static int solenoid_running = 0;

void solenoid_init(void);

void solenoid_run_button(int button_pressed);

#endif /*SOLENOID_H*/