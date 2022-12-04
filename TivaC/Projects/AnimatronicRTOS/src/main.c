/**
 * This file is part of Animatronic Control project which is released under GPLv3 license.
 * See file LICENSE.txt or go to https://www.gnu.org/licenses/gpl-3.0.txt for full license details.
 */

/*******************************************************************************
* Project: Animatronic Control
* Brief: Project developed to control an animatronic with 8 servos, works with
*        Animatronic Control Interface project
* Authors: Rebeca Caetano Fiodi, Yuri Andreiko
* Year: 2022
*******************************************************************************/

/**
 * File: main.c
 * Brief: Initializes all RTOS enviroment
 * Author: Yuri Andreiko
 * Year: 2022
*/

#include <stdint.h>
#include "system_tm4c1294.h" // CMSIS-Core
#include "cmsis_os2.h" // CMSIS-RTOS
#include "UART_funcs.h"  //UART functions
#include "PWM_funcs.h"  //PWM functions 
#include "servo_funcs.h"
#include "communication.h"

void main(void){
  SystemInit();
  UART_init();
  PWM_init();

  osKernelInitialize();
  servos_thread_init();
  communication_thread_init();

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1)
  {
    
  }
} // main
