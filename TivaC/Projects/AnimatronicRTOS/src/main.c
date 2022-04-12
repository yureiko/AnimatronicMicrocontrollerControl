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

#define MSGQUEUE_OBJECTS      5 //quantidade de mensagens na fila

osThreadId_t UART_thread_id, PWM_thread_id;
osThreadId_t Servo_1_thread_id, Servo_2_thread_id;

osMessageQueueId_t DutyCycle_msg;
osMessageQueueId_t Servo1Position_msg;
osMessageQueueId_t Servo2Position_msg;

void PWM_thread(void *arg)
{
  float duty_cycle;
  osStatus_t status;

  servo_struct *servo_0 = servo_initialize("servo 0", PWM_OUT_1, 0.0);
  servo_set_position(servo_0,0);

  while(1) {

    /*status = osMessageQueueGet(DutyCycle_msg, &duty_cycle, NULL, 1);  // wait for message
    if (status == osOK) {
      
        //PWM_set_duty(duty_cycle);
    }*/
  } 
}

void Servo1_thread(void *arg)
{
  float servo_pos;
  servo_struct *servo = servo_initialize("servo 0", PWM_OUT_1, 90.0);
  servo_set_position(servo, servo->default_position);

  osStatus_t status;

  while(1)
  {
    status = osMessageQueueGet(Servo1Position_msg, &servo_pos, NULL, 1);

    if (status == osOK) 
    {
        servo_set_position(servo, servo_pos);
    }
  }
}

void main(void){
  SystemInit();
  UART_init();
  PWM_init();

  osKernelInitialize();
  
  UART_thread_id = osThreadNew(UART_task, NULL, NULL);
  PWM_thread_id = osThreadNew(PWM_thread, NULL, NULL);

  Servo_1_thread_id = PWM_thread_id = osThreadNew(Servo1_thread, NULL, NULL);
  //Servo_2_thread_id = PWM_thread_id = osThreadNew(Servo2_thread, NULL, NULL);
  
  DutyCycle_msg = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(float), NULL);

  Servo1Position_msg = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(float), NULL);
  Servo2Position_msg = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(float), NULL);

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1)
  {
    
  }
} // main
