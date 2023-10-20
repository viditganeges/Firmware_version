#include "main.h"

#define MINIMUM_PULSE_FREQUENCY                 800U                  //5000U           //8U
#define MAXIMUM_PULSE_FREQUENCY                 200000U

#define DEFAULT_STEPPER_DUTY_CYCLE             50

static uint8_t StepperDutyCycle = DEFAULT_STEPPER_DUTY_CYCLE;

static uint32_t pulseFrequency = MINIMUM_PULSE_FREQUENCY;

/* This section lists the other files that are included in this file.
 */


bool Motor1PwmInit(void)                                   //done//
{  
    return true;
}

bool UpdatePulseFrequencyAndRun(uint32_t freq)       //done//
{
    SetPwmDutyCycle(50);
    pulseFrequency = freq;
    PWM1_ChannelPeriodSet(STEPPER_PWM_CHANNEL,(37500000/pulseFrequency));
    StopPwm();
    StartPwm();

    return true;
}

ErrorStatus StartPulse(void)                                //done//
{
    SetPwmDutyCycle(50);
    StopPwm(); 
    StartPwm();
    return SUCCESS;
}

ErrorStatus StopPulse(void)                          //done//
{
    SetPwmDutyCycle(100);

    return SUCCESS;
}

ErrorStatus SetPulseFrequency(uint32_t frequency)           //done//
{
    ErrorStatus status = SUCCESS;
    if(frequency < MINIMUM_PULSE_FREQUENCY)
    {
        pulseFrequency = MINIMUM_PULSE_FREQUENCY;
        StopPulse();
    }
    else if(frequency >= MINIMUM_PULSE_FREQUENCY && frequency <= MAXIMUM_PULSE_FREQUENCY)
    {
        pulseFrequency = (uint32_t)(frequency);
        status = SUCCESS;
    }
    else
    {
        status = ERROR;
    }
    return status;
}

uint32_t GetPulseFrequency(void)                        //done//
{
    return (pulseFrequency);
}

ErrorStatus SetPwmDutyCycle (uint16_t dutyCycle)    //done//
{
    ErrorStatus status = SUCCESS;
    int16_t duty = dutyCycle;
    if(duty >= 0 && duty <= 100)
    {
        StepperDutyCycle = (uint8_t)duty;
        status = SUCCESS;
    }
    else
    {
        status = ERROR;
    }
    return status;
}

bool SetUpdatePwmDutyCycle (uint16_t dutyCycle) //done//
{
  ErrorStatus status = SUCCESS;
  status = SetPwmDutyCycle(dutyCycle);
  if(status == SUCCESS)
  {
    StartPwm();
  }
  return (status == SUCCESS);
}

uint8_t GetPwmDutyCycle (void)               //done//
{
  return StepperDutyCycle;
}

ErrorStatus StartPwm (void)                  //done//
{
  ErrorStatus status = SUCCESS;
  
  uint32_t period_value = PWM1_ChannelPeriodGet(STEPPER_PWM_CHANNEL);
  PWM1_ChannelDutySet(STEPPER_PWM_CHANNEL,(period_value/100)*(100-StepperDutyCycle));
  
  return status;  
}

ErrorStatus StopPwm (void)                   //done//
{
  ErrorStatus status = SUCCESS;
  
  uint8_t duty_cycle = 0;
  uint32_t period_value = PWM1_REGS->PWM_CH_NUM[3].PWM_CPRD;
  PWM1_ChannelDutySet(PWM_CHANNEL_1,(period_value/100)*(100-duty_cycle));
  
  return status;  
}