#include "PWM_funcs.h"
#include "driverlib/rom.h"

#define DEFINED_SYSTEM_CLOCK SystemCoreClock
extern uint32_t SystemCoreClock;

#define PWM_N_PARAMETER ((uint32_t)((DEFINED_SYSTEM_CLOCK/64.0)*(1.0/PWM_FREQUENCY)))

void PWM_init()
{      
    //
    // The PWM peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    
    //
    // Set the PWM clock to the system clock.
    //
    PWMClockSet(PWM0_BASE, PWM_SYSCLK_DIV_64);

    //
    // Set GPIO enable
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);

    //
    // Configure the GPIO pin muxing to select PWM functions for these pins.
    // This step selects which alternate function is available for these pins.
    // This is necessary if your part supports GPIO pin function muxing.
    //
    GPIOPinConfigure(GPIO_PF0_M0PWM0);
    GPIOPinConfigure(GPIO_PF1_M0PWM1);
    GPIOPinConfigure(GPIO_PF2_M0PWM2);
    GPIOPinConfigure(GPIO_PF3_M0PWM3);
    
    GPIOPinConfigure(GPIO_PG0_M0PWM4);
    GPIOPinConfigure(GPIO_PG1_M0PWM5);
    
    GPIOPinConfigure(GPIO_PK4_M0PWM6);
    GPIOPinConfigure(GPIO_PK5_M0PWM7);

    //
    // Configure the GPIO pad for PWM function
    //
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
    
    GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_0);
    GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_1);
    
    GPIOPinTypePWM(GPIO_PORTK_BASE, GPIO_PIN_4);
    GPIOPinTypePWM(GPIO_PORTK_BASE, GPIO_PIN_5);

    //
    // Configure the PWM0 to count up/down without synchronization.
    // Note: Enabling the dead-band generator automatically couples the 2
    // outputs from the PWM block so we don't use the PWM synchronization.
    //
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
                    PWM_GEN_MODE_NO_SYNC);
    
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN |
                    PWM_GEN_MODE_NO_SYNC);
    
    PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN |
                    PWM_GEN_MODE_NO_SYNC);
    
    PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN |
                    PWM_GEN_MODE_NO_SYNC);

    //
    // Set the PWM period.  To calculate the appropriate parameter
    // use the following equation: N = (1 / f) * SysClk.  Where N is the
    // function parameter, f is the desired frequency, and SysClk is the
    // system clock frequency. Note that
    // the maximum period you can set is 2^16 - 1.
    //
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, PWM_N_PARAMETER);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, PWM_N_PARAMETER);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, PWM_N_PARAMETER);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, PWM_N_PARAMETER);

    //
    // Set PWM duty cycle.  You set the duty cycle as a
    // function of the period.  Since the period was set above, you can use the
    // PWMGenPeriodGet() function.
    //
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,
                     (uint32_t)(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*0.075));
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,
                     (uint32_t)(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0)*0.075));
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,                            
                     (uint32_t)(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1)*0.075));
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3,                            
                     (uint32_t)(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1)*0.075));
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4,                            
                     (uint32_t)(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2)*0.075));
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5,                            
                     (uint32_t)(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_2)*0.075));
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6,                            
                     (uint32_t)(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_3)*0.075));
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7,                            
                     (uint32_t)(PWMGenPeriodGet(PWM0_BASE, PWM_GEN_3)*0.075));

    //
    // Enables the counter for a PWM generator block.
    //
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    PWMGenEnable(PWM0_BASE, PWM_GEN_1);
    PWMGenEnable(PWM0_BASE, PWM_GEN_2);
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
    
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
      
}

void PWM_set_duty(uint32_t PWMOut, float duty)
{
  uint32_t PWMGen;
    
  if(PWMOut == PWM_OUT_0 || PWMOut == PWM_OUT_1)
  {
    PWMGen = PWM_GEN_0;
  }
  else if(PWMOut == PWM_OUT_2 || PWMOut == PWM_OUT_3)
  {
    PWMGen = PWM_GEN_1;
  }
  else if(PWMOut == PWM_OUT_4 || PWMOut == PWM_OUT_5)
  {
    PWMGen = PWM_GEN_2;
  }
  else if(PWMOut == PWM_OUT_6 || PWMOut == PWM_OUT_7)
  {
    PWMGen = PWM_GEN_3;
  }
  else
  {
    return;
  }
  
  PWMPulseWidthSet(PWM0_BASE, PWMOut,
                     (uint32_t)(PWMGenPeriodGet(PWM0_BASE, PWMGen)*(duty/100.0)));
  
}