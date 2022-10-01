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
  // Initializes task variables
  uint8_t data_buffer = 0x00;
  uint8_t payload_size = 0;
  uint8_t index = 0;
  protocol_communication_state_t state = WAITING_FIRST_HEAD_CHAR;

  static servo_message_t servo_message;

  while(1)
  {
    // If there is data to be read
    if(UART1_char_available())
    {
       data_buffer = (uint8_t)(UART1_get_byte() & 0x000000FF);
      
       switch(state)
       {
          case WAITING_FIRST_HEAD_CHAR:
          {
            if(data_buffer == START_MESSAGE_FLAG)
            {
                // First message head is valid, change to wait second valid char state
                state = WAITING_SECOND_HEAD_CHAR;
            }
            break;
          }
          
          case WAITING_SECOND_HEAD_CHAR:
          {
            if(data_buffer == START_MESSAGE_FLAG)
            {
              // Second message head is valid, change to wait spayload char state
              state = WAITING_PAYLOAD_CHAR;
            }
            else
            {
              // Second head char is invalid, back to first state
              state = WAITING_FIRST_HEAD_CHAR;
            }
            break;
          }

          case WAITING_PAYLOAD_CHAR:
          {
            if(data_buffer > 0 && data_buffer <= MAX_PAYLOAD_SIZE)
            {
              // Payload is valid, change to waiting useful data state 
              state = WAITING_DATA_CHAR;
              payload_size = data_buffer;
              index = 0;
            }
            else
            {
              // Payload char is invalid, back to first state
              state = WAITING_FIRST_HEAD_CHAR;
            }
            break;
          }

          case WAITING_DATA_CHAR:
          {
            if(index < payload_size)
            {
              // Saves received useful char into the buffer
              servo_message.data[index] = data_buffer;
              index++;

              // Last data byte
              if(index == payload_size)
              {
                // When the index is equal to the payload, change to wait first ending char state
                state = WAITING_FIRST_END_CHAR;
              }
            }
            else
            {
              // If, for some reason, state wasn't change with last data, change to wait first ending char state
              state = WAITING_FIRST_END_CHAR;
            }
            break;
          }

          case WAITING_FIRST_END_CHAR:
          {
             if(data_buffer == END_MESSAGE_FLAG)
            {
              // First ending char is valid, change to wait second valid char state
              state = WAITING_SECOND_END_CHAR;
            }
            else
            {
              // First ending char is not valid, change to first state
              state = WAITING_FIRST_HEAD_CHAR;
            }
            break;
          }

          case WAITING_SECOND_END_CHAR:
          {
            if(data_buffer == END_MESSAGE_FLAG)
            {
              // Second ending char is valid, send the buffered message in the queue
              servo_message.size = payload_size;
              osMessageQueuePut(servo_main_thread_msg, &servo_message, 0, NULL);
            }

            // Back to first state
            state = WAITING_FIRST_HEAD_CHAR;

            break;
          }
          default:
            // To avoid deadlock
            state = WAITING_FIRST_HEAD_CHAR;
          break;
       }    
    }
  }
}