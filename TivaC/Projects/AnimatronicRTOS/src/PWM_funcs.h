/**
 * This file is part of Animatronic Control project which is released under GPLv3 license.
 * See file LICENSE.txt or go to https://www.gnu.org/licenses/gpl-3.0.txt for full license details.
 */

/**
 * File: PWM_funcs.h
 * Brief: Manage all the 8 PWM channels from TIVA C TM4C1294 microcontroller
 * Author: Yuri Andreiko
 * Year: 2022
*/

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