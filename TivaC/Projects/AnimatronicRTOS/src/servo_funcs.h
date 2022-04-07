#ifndef SERVO_FUNCS_H
#define SERVO_FUNCS_H

#include <stdbool.h>
#include <stdint.h>

typedef struct{
  char id[8];
  uint32_t pwm_id;
  float default_position;
}servo_struct;

void servo_set_position(servo_struct *servo, float pos);

servo_struct *servo_initialize(char *id, uint32_t pwm_id, float default_position);

#endif