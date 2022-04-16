/*******************************************************************************
*Animatronic RTOS
*
*******************************************************************************/

#include <stdint.h>
#include "system_tm4c1294.h" // CMSIS-Core
#include "cmsis_os2.h" // CMSIS-RTOS
#include "UART_funcs.h"  //UART functions
#include "PWM_funcs.h"  //PWM functions 
#include "servo_funcs.h"
#include "communication.h"

osThreadId_t UART_thread_id, PWM_thread_id;

void main(void){
  SystemInit();
  UART_init();
  PWM_init();

  osKernelInitialize();
  servos_thread_init();
  
  UART_thread_id = osThreadNew(UART_task, NULL, NULL);

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1)
  {
    
  }
} // main
