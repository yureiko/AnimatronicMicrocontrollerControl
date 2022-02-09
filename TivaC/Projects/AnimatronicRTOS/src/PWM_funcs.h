//Funções do periférico PWM
//Autor: Yuri Andreiko
//Data: 01/07/19

#ifndef PWM_FUNCS_H
#define PWM_FUNCS_H

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"


void PWM_init();

void PWM_set_duty(uint32_t PWMBase, uint32_t PWMOut, float duty); //recebe o duty cycle

#endif