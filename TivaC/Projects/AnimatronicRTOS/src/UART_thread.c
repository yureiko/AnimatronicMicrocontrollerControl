#include "UART_thread.h"
#include "UART_funcs.h"
#include "cmsis_os2.h" // CMSIS-RTOS

void UART_thread(void *arg)
{
  osStatus_t status;

  while(1)
  {
    while(UART_char_available())                                
    {   


    }
  }
}