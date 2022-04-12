#include "communication.h"
#include "UART_funcs.h"
#include "cmsis_os2.h" // CMSIS-RTOS

#define DATA_BUFFER_SIZE 128
#define START_MESSAGE_FLAG 0xAA
#define END_MESSAGE_FLAG 0x55

static void process_received_data(uint32_t *data);
static void process_payload_data(uint32_t *data, uint8_t payload);

extern osMessageQueueId_t Servo1Position_msg;

void UART_task(void *arg)
{
  static uint32_t data_buffer[DATA_BUFFER_SIZE];
  uint8_t data_index;

  while(1)
  {
    data_index = 0;

    while(UART_char_available())                                
    {      
        data_buffer[data_index] = UART_get_byte() & 0x000000FF;
        
        // Check end of frame condition
        if(data_index > 0 && 
          data_buffer[data_index - 1] & data_buffer[data_index] == END_MESSAGE_FLAG)
          {
            data_index = 0;
            process_received_data(data_buffer);
          }

        data_index++;
        if(data_index == DATA_BUFFER_SIZE)
        {
          data_index = 0;
        }
    }
  }
}

void process_received_data(uint32_t *data)
{
  uint8_t data_index = 0xFF;
  uint8_t payload_size = 0xFF;

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

void process_payload_data(uint32_t *data, uint8_t payload)
{
  uint8_t id = (uint8_t)(*data & 0x000000FF);
  float value;
  
  switch(id)
  {
    case 0x00:
      value = (float) (0x000000FF & data[2]);
      osMessageQueuePut(Servo1Position_msg, &value, 0, NULL);
    break;

    case 0x01:
    break;

    default:
    break;
  }
}