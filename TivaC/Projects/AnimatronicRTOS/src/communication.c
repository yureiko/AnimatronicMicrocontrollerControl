#include "communication.h"
#include "UART_funcs.h"
#include "cmsis_os2.h" // CMSIS-RTOS

#define DATA_BUFFER_SIZE 128
#define START_MESSAGE_FLAG 0xAA
#define END_MESSAGE_FLAG 0x55

void process_received_data_task(void *arg);
static void process_payload_data(uint8_t *data, uint8_t payload);

extern osMessageQueueId_t Servo1Position_msg;

osMessageQueueId_t CommunicationData_msg;
osThreadId_t CommunicationData_thread;

typedef struct
{
  uint8_t msg_size;
  uint8_t *msg;
}Communication_data_struct;

void UART_task(void *arg)
{
  static uint8_t data_buffer[DATA_BUFFER_SIZE];
  uint8_t data_index;

  CommunicationData_thread =  osThreadNew(process_received_data_task, NULL, NULL);
  CommunicationData_msg = osMessageQueueNew(5, sizeof(Communication_data_struct), NULL);

  Communication_data_struct communication_data;
  communication_data.msg = data_buffer;

  while(1)
  {
    data_index = 0;

    while(UART_char_available())                                
    {      
        data_buffer[data_index] = (uint8_t)(UART_get_byte() & 0x000000FF);
        
        // Check end of frame condition
        if(data_index > 0 && 
          data_buffer[data_index - 1] == END_MESSAGE_FLAG && 
          data_buffer[data_index] == END_MESSAGE_FLAG)
          {
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

  uint8_t *data;

  while(1)
  {
    status = osMessageQueueGet(CommunicationData_msg, &communication_data, NULL, 1);
    data = communication_data.msg;

    for(data_index = 0; data_index < DATA_BUFFER_SIZE; data_index ++)
    {
      if(data[data_index] == START_MESSAGE_FLAG && 
        data[data_index + 1] == START_MESSAGE_FLAG )
      {
        payload_size = data[data_index + 2];

        if(data[data_index + payload_size + 3] == END_MESSAGE_FLAG && 
          data[data_index + payload_size + 4] == END_MESSAGE_FLAG )
          {
            process_payload_data(&data[data_index + 3], payload_size);
          }
      }
    }
  }
}

void process_payload_data(uint8_t *data, uint8_t payload)
{
  uint8_t id =  *data;
  float value;
  
  switch(id)
  {
    case 0x00:
      value = (float) ((0x000000FF & data[2] | 0x0000FF00 & data[3] << 8))/100.0;
      osMessageQueuePut(Servo1Position_msg, &value, 0, NULL);
    break;

    case 0x01:
    break;

    default:
    break;
  }
}