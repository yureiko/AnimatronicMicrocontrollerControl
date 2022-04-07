#include "communication.h"
#include "UART_funcs.h"
#include "cmsis_os2.h" // CMSIS-RTOS

#define DATA_BUFFER_SIZE 128
#define START_MESSAGE_FLAG 0xAA
#define END_MESSAGE_FLAG 0x55

#define FIRST_BYTE_START_MESSAGE_POS(x) x
#define SECOND_BYTE_START_MESSAGE_POS(x) x + 1

#define BYTE_PAYLOAD_SIZE_POS(x) x + 2
#define BYTE_START_PAYLOAD_POS(x) x + 3

#define FIRST_BYTE_END_MESSAGE_POS(x, payload_size) x + payload_size + 3
#define SECOND_BYTE_END_MESSAGE_POS(x, payload_size) x + payload_size + 4

static void process_received_data(uint8_t *data);
static void process_payload_data(uint8_t *data, uint8_t payload);

void UART_task(void *arg)
{
  uint8_t data_buffer[DATA_BUFFER_SIZE];
  uint8_t data_index;

  while(1)
  {
    data_index = 0;

    while(UART_char_available())                                
    {      
        data_buffer[data_index] = UART_get_byte();
        
        // Check end of frame condition
        if(data_index > 0 && 
          data_buffer[data_index - 1] & data_buffer[data_index] == END_MESSAGE_FLAG)
          {
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

void process_received_data(uint8_t *data)
{
  uint8_t data_index = 0xFF;
  uint8_t payload_size = 0xFF;

  for(data_index = 0; data_index < DATA_BUFFER_SIZE; data_index ++)
  {
    if(data[FIRST_BYTE_START_MESSAGE_POS(data_index)] & 
       data[SECOND_BYTE_START_MESSAGE_POS(data_index)] == START_MESSAGE_FLAG )
    {
      payload_size = data[data_index + 1];

      if(data[FIRST_BYTE_END_MESSAGE_POS(data_index, payload_size)] & 
        data[SECOND_BYTE_END_MESSAGE_POS(data_index, payload_size)] == END_MESSAGE_FLAG )
        {
          process_payload_data(&data[BYTE_START_PAYLOAD_POS(data_index)], payload_size);
        }
    }
  }
}

void process_payload_data(uint8_t *data, uint8_t payload)
{

}