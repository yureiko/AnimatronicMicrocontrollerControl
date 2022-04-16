#ifndef SERVO_FUNCS_H
#define SERVO_FUNCS_H

#include <stdbool.h>
#include <stdint.h>
#include "cmsis_os2.h" // CMSIS-RTOS

typedef struct
{
  uint8_t size;
  uint8_t *data;
}servo_message_t;

void servos_thread_init();

#endif