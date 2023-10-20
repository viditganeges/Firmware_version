#include "main.h"

#define DEFAULT_MOTOR1_FORWARD_DUTY_CYCLE             0
#define DEFAULT_MOTOR1_REVERSE_DUTY_CYCLE             0

static uint8_t Motor1ForwardDutyCycle = DEFAULT_MOTOR1_FORWARD_DUTY_CYCLE;
static uint8_t Motor1ReverseDutyCycle = DEFAULT_MOTOR1_REVERSE_DUTY_CYCLE;

bool Motor1PwmInit(void)        //done//
{  
  PWM0_ChannelsStart(PWM_CHANNEL_1_MASK);
  
  if(SetMotor1ForwardDutyCycle(Motor1ForwardDutyCycle) != SUCCESS)
  {
    return false;
  }
  if(SetMotor1ReverseDutyCycle(Motor1ReverseDutyCycle) != SUCCESS)
  {
    return false;
  }
  
  return true;
}

ErrorStatus SetMotor1ForwardDutyCycle (uint16_t dutyCycle)      //done//
{
  ErrorStatus status = SUCCESS;
  int16_t duty = dutyCycle;
  if(duty >= 0 && duty <= 100)
  {
    Motor1ForwardDutyCycle = (uint8_t)duty;
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }
  return status;
}

ErrorStatus SetMotor1ReverseDutyCycle (uint16_t dutyCycle)      //done//
{
  ErrorStatus status = SUCCESS;
  int16_t duty = dutyCycle;
  if(duty >= 0 && duty <= 100)
  {
    Motor1ReverseDutyCycle = (uint8_t)duty;
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }
  return status;
}

bool SetUpdateMotor1ForwardDutyCycle (uint16_t dutyCycle)
{
  ErrorStatus status = SUCCESS;
  status = SetMotor1ForwardDutyCycle(dutyCycle);
  if(status == SUCCESS)
  {
    StartMotor1Forward();
  }
  return (status == SUCCESS);
}

bool SetUpdateMotor1ReverseDutyCycle (uint16_t dutyCycle)
{
  ErrorStatus status = SUCCESS;
  status = SetMotor1ReverseDutyCycle(dutyCycle);
  if(status == SUCCESS)
  {
    StartMotor1Reverse();
  }
  return (status == SUCCESS);
}

uint8_t GetMotor1ForwardDutyCycle (void)
{
  return Motor1ForwardDutyCycle;
}

uint8_t GetMotor1ReverseDutyCycle (void)
{
  return Motor1ReverseDutyCycle;
}

ErrorStatus StartMotor1Forward (void)   //done//
{
  ErrorStatus status = SUCCESS;
  
  PWM0_ChannelDutySet(MOTOR1_PWM_CHANNEL, (40000*Motor1ForwardDutyCycle)/100);
  
  return status;  
}

ErrorStatus StartMotor1Reverse (void)   //done//
{
  ErrorStatus status = SUCCESS;
  
//  PWM0_ChannelDutySet(MOTOR1_PWM_CHANNEL, 40000-((40000*Motor1ReverseDutyCycle)/100));
  PWM0_ChannelDutySet(MOTOR1_PWM_CHANNEL, (40000*Motor1ReverseDutyCycle)/100);
  
  return status;  
}

ErrorStatus StopMotor1 (void)    //done//
{
  ErrorStatus status = SUCCESS;
  
  PWM0_ChannelDutySet(MOTOR1_PWM_CHANNEL, 0);
  MOTORA_IN1_Clear();
  MOTORA_PC8_nsleep_Clear(); 
  
  return status;  
}

