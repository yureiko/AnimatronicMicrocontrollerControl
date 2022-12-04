/**
 * This file is part of Animatronic Control project which is released under GPLv3 license.
 * See file LICENSE.txt or go to https://www.gnu.org/licenses/gpl-3.0.txt for full license details.
 */

/**
 * File: communication.c
 * Brief: Manages all received messages from UART, validates and sends data to servo message queue 
 * Author: Yuri Andreiko
 * Year: 2022
*/

#include "communication.h"
#include "UART_funcs.h"
#include "servo_funcs.h"
#include "cmsis_os2.h" // CMSIS-RTOS
#include <stdio.h>

#define START_MESSAGE_FLAG 0xAA
#define END_MESSAGE_FLAG 0x55
#define MAX_PAYLOAD_SIZE 5

typedef enum{
  WAITING_FIRST_HEAD_CHAR,
  WAITING_SECOND_HEAD_CHAR,
  WAITING_PAYLOAD_CHAR,
  WAITING_DATA_CHAR,
  WAITING_FIRST_END_CHAR,
  WAITING_SECOND_END_CHAR
} protocol_communication_state_t;

osThreadId_t UART0_thread_id;
osThreadId_t UART1_thread_id;

extern osMessageQueueId_t servo_message_queue;

typedef struct
{
  uint16_t msg_size;
  uint8_t *msg;
}Communication_data_struct;

typedef int32_t (*function_int32)();
typedef uint8_t (*function_uint8)();

typedef struct
{
  function_uint8 UART_char_available_function;
  function_int32 UART_get_char_function;
  uint8_t data_buffer;
  uint8_t payload_size;
  uint8_t index;
  protocol_communication_state_t state;
  servo_message_t servo_message;
}Communication_task_control;

void UART0_task(void *arg);
void UART1_task(void *arg);
void communication_read_messages(Communication_task_control *task_control);

void communication_thread_init()
{
  UART0_thread_id = osThreadNew(UART0_task, NULL, NULL);
  UART1_thread_id = osThreadNew(UART1_task, NULL, NULL);
}

void UART0_task(void *arg)
{
  static Communication_task_control uart0_task_control = {
    .UART_char_available_function = UART0_char_available,
    .UART_get_char_function = UART0_get_byte,
    .data_buffer = 0,
    .payload_size = 0,
    .index = 0,
    .state = WAITING_FIRST_HEAD_CHAR
  };

  while(1)
  {
    communication_read_messages(&uart0_task_control);
  }
}

void UART1_task(void *arg)
{
  static Communication_task_control uart1_task_control = {
    .UART_char_available_function = UART1_char_available,
    .UART_get_char_function = UART1_get_byte,
    .data_buffer = 0,
    .payload_size = 0,
    .index = 0,
    .state = WAITING_FIRST_HEAD_CHAR
  };

  while(1)
  {
    communication_read_messages(&uart1_task_control);
  }
}

void communication_read_messages(Communication_task_control *task_control)
{
  servo_message_t *servo_message = &(task_control->servo_message);
  // If there is data to be read
    if(task_control->UART_char_available_function())
    {
       task_control->data_buffer = (uint8_t)(task_control->UART_get_char_function() & 0x000000FF);
      
       switch(task_control->state)
       {
          case WAITING_FIRST_HEAD_CHAR:
          {
            if(task_control->data_buffer == START_MESSAGE_FLAG)
            {
                // First message head is valid, change to wait second valid char state
                task_control->state = WAITING_SECOND_HEAD_CHAR;
            }
            break;
          }
          
          case WAITING_SECOND_HEAD_CHAR:
          {
            if(task_control->data_buffer == START_MESSAGE_FLAG)
            {
              // Second message head is valid, change to wait spayload char state
              task_control->state = WAITING_PAYLOAD_CHAR;
            }
            else
            {
              // Second head char is invalid, back to first state
              task_control->state = WAITING_FIRST_HEAD_CHAR;
            }
            break;
          }

          case WAITING_PAYLOAD_CHAR:
          {
            if(task_control->data_buffer > 0 && task_control->data_buffer <= MAX_PAYLOAD_SIZE)
            {
              // Payload is valid, change to waiting useful data state 
              task_control->state = WAITING_DATA_CHAR;
              task_control->payload_size = task_control->data_buffer;
              task_control->index = 0;
            }
            else
            {
              // Payload char is invalid, back to first state
              task_control->state = WAITING_FIRST_HEAD_CHAR;
            }
            break;
          }

          case WAITING_DATA_CHAR:
          {
            if(task_control->index < task_control->payload_size)
            {
              // Saves received useful char into the buffer
              servo_message->data[task_control->index] = task_control->data_buffer;
              task_control->index++;

              // Last data byte
              if(task_control->index == task_control->payload_size)
              {
                // When the index is equal to the payload, change to wait first ending char state
                task_control->state = WAITING_FIRST_END_CHAR;
              }
            }
            else
            {
              // If, for some reason, state wasn't change with last data, change to wait first ending char state
              task_control->state = WAITING_FIRST_END_CHAR;
            }
            break;
          }

          case WAITING_FIRST_END_CHAR:
          {
             if(task_control->data_buffer == END_MESSAGE_FLAG)
            {
              // First ending char is valid, change to wait second valid char state
              task_control->state = WAITING_SECOND_END_CHAR;
            }
            else
            {
              // First ending char is not valid, change to first state
              task_control->state = WAITING_FIRST_HEAD_CHAR;
            }
            break;
          }

          case WAITING_SECOND_END_CHAR:
          {
            if(task_control->data_buffer == END_MESSAGE_FLAG)
            {
              // Second ending char is valid, send the buffered message in the queue
              servo_message->size = task_control->payload_size;
              osMessageQueuePut(servo_message_queue, servo_message, 0, NULL);
            }

            // Back to first state
            task_control->state = WAITING_FIRST_HEAD_CHAR;

            break;
          }
          default:
            // To avoid deadlock
            task_control->state = WAITING_FIRST_HEAD_CHAR;
          break;
       }    
    }
}