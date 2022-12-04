/**
 * This file is part of Animatronic Control project which is released under GPLv3 license.
 * See file LICENSE.txt or go to https://www.gnu.org/licenses/gpl-3.0.txt for full license details.
 */

/**
 * File: UART_funcs.h
 * Brief: Manages UART communication
 * Author: Yuri Andreiko
 * Year: 2022
*/

#ifndef UART_FUNCS_H
#define	UART_FUNCS_H

#include <stdint.h>

void UART_init();

extern uint32_t SystemCoreClock; 

void UART0_send_byte(const uint8_t *pui8Buffer, uint32_t ui32Count);

uint8_t UART0_char_available();

int32_t UART0_get_byte();

void UART1_send_byte(const uint8_t *pui8Buffer, uint32_t ui32Count);

uint8_t UART1_char_available();

int32_t UART1_get_byte();

#endif
