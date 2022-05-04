#include "communication.h"
#include "UART_funcs.h"
#include "servo_funcs.h"
#include "cmsis_os2.h" // CMSIS-RTOS
#include <stdio.h>

#define DATA_BUFFER_SIZE 30
#define START_MESSAGE_FLAG 0xAA
#define END_MESSAGE_FLAG 0x55

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
  uint8_t data_buffer[DATA_BUFFER_SIZE];
  uint16_t data_index = 0;
  uint16_t n_bytes = 0;
  uint8_t valid_head = false;
  uint8_t valid_end_frame = false;
  uint8_t payload_size = 0;
  servo_message_t servo_message;

  //
  Communication_data_struct communication_data;
  communication_data.msg = data_buffer;

  while(1)
  {
    if(UART_char_available())
    {
       data_buffer[data_index] = (uint8_t)(UART_get_byte() & 0x000000FF);
       data_index++;
       n_bytes++;

       if(n_bytes >= DATA_BUFFER_SIZE)
       {
         n_bytes --;
       }
        
       if(data_index >= DATA_BUFFER_SIZE)
       {
          data_index = 0;
       }
    }
      // Search for valid frame
    if(n_bytes > 5)
    {
      uint16_t i;

      i = (data_index >= n_bytes) ?  data_index - n_bytes : (DATA_BUFFER_SIZE - (n_bytes - data_index));
      
      // If data_index is equal, starts in 0
      i = data_index == n_bytes ? 0 : i;

      valid_head = false;
      payload_size = 0;

      // Check if the frame has the correct start message format 
      valid_head = data_buffer[i] == START_MESSAGE_FLAG;
      i = i < DATA_BUFFER_SIZE - 1 ? i+1 : 0;

      valid_head &= data_buffer[i] == START_MESSAGE_FLAG;
      i = i < DATA_BUFFER_SIZE - 1 ? i+1 : 0;

      if(valid_head)
      {
        payload_size = data_buffer[i];
        i = i < DATA_BUFFER_SIZE - 1 ? i+1 : 0;

        if(n_bytes >= payload_size + 5)
        {
          uint16_t k = payload_size;
          servo_message.size = payload_size;
          
          while(k)
          {
            servo_message.data[payload_size - k] = data_buffer[i + payload_size - k < DATA_BUFFER_SIZE ? i + payload_size - k : DATA_BUFFER_SIZE - 1 - i + payload_size - k ];
            k--;
          }

          i = i + payload_size >= DATA_BUFFER_SIZE - 1 ? (i + payload_size -1) - (DATA_BUFFER_SIZE - 1) : i + payload_size;

          valid_end_frame = data_buffer[i] == END_MESSAGE_FLAG;
          i = i < DATA_BUFFER_SIZE - 1 ? i+1 : 0;

          valid_end_frame &= data_buffer[i] == END_MESSAGE_FLAG;

          if(valid_end_frame)
          {           
            osMessageQueuePut(servo_main_thread_msg, &servo_message, 0, NULL);
            data_buffer[i] = 0x00;
          }
          
          n_bytes -= payload_size + 5;
        }
      }
    }
  }
}