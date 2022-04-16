#include "communication.h"
#include "UART_funcs.h"
#include "servo_funcs.h"
#include "cmsis_os2.h" // CMSIS-RTOS

#define DATA_BUFFER_SIZE 128
#define START_MESSAGE_FLAG 0xAA
#define END_MESSAGE_FLAG 0x55

void process_received_data_task(void *arg);

osMessageQueueId_t CommunicationData_msg;
osThreadId_t CommunicationData_thread;

extern osMessageQueueId_t servo_main_thread_msg;

typedef struct
{
  uint8_t msg_size;
  uint8_t *msg;
}Communication_data_struct;

void UART_task(void *arg)
{
  // Buffer for received data from UART
  static uint8_t data_buffer[DATA_BUFFER_SIZE];
  uint8_t data_index;

  // Start the communication task, responsible to handle messages from user interface
  CommunicationData_thread =  osThreadNew(process_received_data_task, NULL, NULL);
  CommunicationData_msg = osMessageQueueNew(5, sizeof(Communication_data_struct), NULL);

  //
  Communication_data_struct communication_data;
  communication_data.msg = data_buffer;

  while(1)
  {
    if(UART_char_available())                                
    {      
        data_buffer[data_index] = (uint8_t)(UART_get_byte() & 0x000000FF);
        
        // Check end of frame condition
        if(data_index > 0 && 
          data_buffer[data_index - 1] == END_MESSAGE_FLAG && 
          data_buffer[data_index] == END_MESSAGE_FLAG)
          {
            // Send the message size
            communication_data.msg_size = data_index + 1;

            osMessageQueuePut(CommunicationData_msg, &communication_data, 0, NULL);

            data_index = 0;
          }

        data_index++;
        if(data_index == DATA_BUFFER_SIZE)
        {
          data_index = 0;
        }
    }
  }
}

void process_received_data_task(void* arg)
{
  uint8_t data_index = 0xFF;
  uint8_t payload_size = 0xFF;

  osStatus_t status;
  Communication_data_struct communication_data;
  servo_message_t servo_message;

  uint8_t *data;

  while(1)
  {
    status = osMessageQueueGet(CommunicationData_msg, &communication_data, NULL, osWaitForever);
    data = communication_data.msg;

    // Search for valid frame
    for(data_index = 0; data_index < communication_data.msg_size; data_index ++)
    {
      // Check if the frame has the correct start message format 
      if(data[data_index] == START_MESSAGE_FLAG && 
        data[data_index + 1] == START_MESSAGE_FLAG )
      {
        // Save the payload size
        payload_size = data[data_index + 2];

        // Check if the frame has the correct end message format
        if(data[data_index + payload_size + 3] == END_MESSAGE_FLAG && 
          data[data_index + payload_size + 4] == END_MESSAGE_FLAG )
          {
            servo_message.size = payload_size;
            servo_message.data = &data[data_index + 3];

            osMessageQueuePut(servo_main_thread_msg, &servo_message, 0, NULL);

            break;
          }
      }
    }
  }
}