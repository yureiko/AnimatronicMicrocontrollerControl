#ifndef PWM_FUNCS_H
#define PWM_FUNCS_H

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

#define PWM_FREQUENCY 50.0

void PWM_init();

void PWM_set_duty(uint32_t PWMOut, float duty);

void PWM_synchronyze();

#endif