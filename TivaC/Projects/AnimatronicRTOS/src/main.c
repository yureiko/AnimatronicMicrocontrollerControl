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
//#include "inc/hw_memmap.h"

#define MSGQUEUE_OBJECTS      5 //quantidade de mensagens na fila

osThreadId_t UART_thread_id, PWM_thread_id;

osMessageQueueId_t DutyCycle_msg;

void UART_thread(void *arg)
{
  osStatus_t status;

  while(1)
  {
    while(UART_char_available())                                
    {      
        osDelay(500);
    }
  }
}

void PWM_thread(void *arg)
{
  float duty_cycle;
  osStatus_t status;
  while(1) {
    status = osMessageQueueGet(DutyCycle_msg, &duty_cycle, NULL, 1);  // wait for message
    if (status == osOK) {
      
        //PWM_set_duty(duty_cycle);
    }
  } 
}

void main(void){
  SystemInit();
  UART_init();
  PWM_init();

  osKernelInitialize();
  
  UART_thread_id = osThreadNew(UART_thread, NULL, NULL);
  PWM_thread_id = osThreadNew(PWM_thread, NULL, NULL);
  
  DutyCycle_msg = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(float), NULL);

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1)
  {
    
  }
} // main
