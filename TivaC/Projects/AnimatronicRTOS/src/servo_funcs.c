#include "servo_funcs.h"
#include "pwm_funcs.h"
#include <string.h>
#include <stdlib.h>

#define SERVO_MIN_POSITION -90.0                // -90 degrees
#define SERVO_MAX_POSITION 90.0                 // 90 degrees

#define SERVO_MIN_DUTY_CYCLE_PERIOD_MS 1        // 1ms
#define SERVO_MAX_DUTY_CYCLE_PERIOD_MS 2        // 2ms

/******************************************************************************
* Local prototypes
******************************************************************************/
float servo_deg_to_duty_cycle(float pos);

void servo_set_position(servo_struct *servo, float pos)
{
  PWM_set_duty(servo->pwm_id, servo_deg_to_duty_cycle(pos));
}

servo_struct *servo_initialize(char *id, uint32_t pwm_id)
{
  servo_struct *new_servo = NULL;
  
  new_servo = malloc(sizeof(servo_struct));
  
  strcpy(new_servo->id, id);
  new_servo->pwm_id = pwm_id;

  return new_servo;
}

float servo_deg_to_duty_cycle(float pos)
{
  float duty;
  float period_ms;
  
  period_ms = SERVO_MIN_DUTY_CYCLE_PERIOD_MS + ((pos - SERVO_MIN_POSITION)  * 
                      ((float)(SERVO_MAX_DUTY_CYCLE_PERIOD_MS - SERVO_MIN_DUTY_CYCLE_PERIOD_MS)) /
                      ((float)(SERVO_MAX_POSITION - SERVO_MIN_POSITION)));

  duty = (period_ms/10.f) * PWM_FREQUENCY;

  return duty;
}
