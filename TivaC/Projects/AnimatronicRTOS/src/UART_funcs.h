#ifndef UART_FUNCS_H
#define	UART_FUNCS_H

#include <stdint.h>

void UART_init();

void UART_send_byte(const uint8_t *pui8Buffer, uint32_t ui32Count);

extern uint32_t SystemCoreClock; 

uint8_t UART_char_available();

int32_t UART_get_byte();


#endif
