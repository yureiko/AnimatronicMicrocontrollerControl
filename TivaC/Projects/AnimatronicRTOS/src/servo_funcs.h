/**
 * This file is part of Animatronic Control project which is released under GPLv3 license.
 * See file LICENSE.txt or go to https://www.gnu.org/licenses/gpl-3.0.txt for full license details.
 */

/**
 * File: servo_funcs.h
 * Brief: Manages received messages and translates to PWM duty cycle value for PWM outputs
 * Author: Yuri Andreiko
 * Year: 2022
*/

#ifndef SERVO_FUNCS_H
#define SERVO_FUNCS_H

#include <stdbool.h>
#include <stdint.h>
#include "cmsis_os2.h" // CMSIS-RTOS

typedef struct
{
  uint8_t size;
  uint8_t data[16];
}servo_message_t;

void servos_thread_init();

#endif