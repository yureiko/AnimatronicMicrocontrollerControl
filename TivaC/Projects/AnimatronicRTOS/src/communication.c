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

void process_received_data_task(void *arg);

osMessageQueueId_t CommunicationData_msg;
osThreadId_t CommunicationData_thread;

extern osMessageQueueId_t servo_main_thread_msg;

typedef struct
{
  uint16_t msg_size;
  uint8_t *msg;
}Communication_data_struct;

void UART_task(void *arg)
{
  // Buffer for received data from UART
  uint8_t data_buffer;
  uint8_t payload_size;
  uint8_t index;
  protocol_communication_state_t state = WAITING_FIRST_HEAD_CHAR;

  static servo_message_t servo_message;

  while(1)
  {
    if(UART_char_available())
    {
       data_buffer = (uint8_t)(UART_get_byte() & 0x000000FF);
      
       switch(state)
       {
          case WAITING_FIRST_HEAD_CHAR:
          {
            if(data_buffer == START_MESSAGE_FLAG)
            {
                state = WAITING_SECOND_HEAD_CHAR;
            }
            break;
          }
          
          case WAITING_SECOND_HEAD_CHAR:
          {
            if(data_buffer == START_MESSAGE_FLAG)
            {
              state = WAITING_PAYLOAD_CHAR;
            }
            else
            {
              state = WAITING_FIRST_HEAD_CHAR;
            }
            break;
          }

          case WAITING_PAYLOAD_CHAR:
          {
            if(data_buffer > 0 && data_buffer <= MAX_PAYLOAD_SIZE)
            {
              state = WAITING_DATA_CHAR;
              payload_size = data_buffer;
              index = 0;
            }
            else
            {
              state = WAITING_FIRST_HEAD_CHAR;
            }
            break;
          }

          case WAITING_DATA_CHAR:
          {
            if(index < payload_size)
            {
              servo_message.data[index] = data_buffer;
              index++;

              // Last data byte
              if(index == payload_size)
              {
                state = WAITING_FIRST_END_CHAR;
              }
            }
            else
            {
              state = WAITING_FIRST_END_CHAR;
            }
            break;
          }

          case WAITING_FIRST_END_CHAR:
          {
             if(data_buffer == END_MESSAGE_FLAG)
            {
              state = WAITING_SECOND_END_CHAR;
            }
            else
            {
              state = WAITING_FIRST_HEAD_CHAR;
            }
            break;
          }

          case WAITING_SECOND_END_CHAR:
          {
            if(data_buffer == END_MESSAGE_FLAG)
            {
              servo_message.size = payload_size;
              osMessageQueuePut(servo_main_thread_msg, &servo_message, 0, NULL);
            }

            state = WAITING_FIRST_HEAD_CHAR;

            break;
          }
          default:
            state = WAITING_FIRST_HEAD_CHAR;
          break;
       }    
    }
  }
}