#include "main.h"

#define DEFAULT_MOTOR2_FORWARD_DUTY_CYCLE             0
#define DEFAULT_MOTOR2_REVERSE_DUTY_CYCLE             0

static uint8_t Motor2ForwardDutyCycle = DEFAULT_MOTOR2_FORWARD_DUTY_CYCLE;
static uint8_t Motor2ReverseDutyCycle = DEFAULT_MOTOR2_REVERSE_DUTY_CYCLE;


bool Motor2PwmInit(void)    //done//
{   
  PWM0_ChannelsStart(PWM_CHANNEL_0_MASK);
    
  if(SetMotor2ForwardDutyCycle(Motor2ForwardDutyCycle) != SUCCESS)
  {
//    Error_Handler();
    return false;
  }
  
  if(SetMotor2ReverseDutyCycle(Motor2ReverseDutyCycle) != SUCCESS)
  {
//    Error_Handler();
    return false;
  }
  
  return true;
}

ErrorStatus SetMotor2ForwardDutyCycle (uint32_t dutyCycle)  //done//
{
  ErrorStatus status = SUCCESS;
  int16_t duty = dutyCycle;
  if(duty >= 0 && duty <= 100)
  {
    Motor2ForwardDutyCycle = (uint8_t)duty;
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }
  return status;
}

ErrorStatus SetMotor2ReverseDutyCycle (uint32_t dutyCycle)  //done//
{
  ErrorStatus status = SUCCESS;
  int16_t duty = dutyCycle;
  if(duty >= 0 && duty <= 100)
  {
    Motor2ReverseDutyCycle = (uint8_t)duty;
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }
  return status;
}

ErrorStatus SetUpdateMotor2ForwardDutyCycle (uint32_t dutyCycle)    //done//
{
  ErrorStatus status = SUCCESS;
  status = SetMotor2ForwardDutyCycle(dutyCycle);
  if(status == SUCCESS)
  {
    StartMotor2Forward();
  }
  return status;
}

ErrorStatus SetUpdateMotor2ReverseDutyCycle (uint32_t dutyCycle)    //done//
{
  ErrorStatus status = SUCCESS;
  status = SetMotor2ReverseDutyCycle(dutyCycle);
  if(status == SUCCESS)
  {
    StartMotor2Reverse();
  }
  return status;
}

uint8_t GetMotor2ForwardDutyCycle (void)    //done//
{
  return Motor2ForwardDutyCycle;
}

uint8_t GetMotor2ReverseDutyCycle (void)    //done//
{
  return Motor2ReverseDutyCycle;
}

ErrorStatus StartMotor2Forward (void)   //done//
{
  ErrorStatus status = SUCCESS;
  
  PWM0_ChannelDutySet(MOTOR2_PWM_CHANNEL, (40000*Motor2ForwardDutyCycle)/100);
  
  return status;  
}

ErrorStatus StartMotor2Reverse (void)   //done//
{
  ErrorStatus status = SUCCESS;
  
  PWM0_ChannelDutySet(MOTOR2_PWM_CHANNEL, 40000-((40000*Motor2ReverseDutyCycle)/100));
  
  return status;  
}

ErrorStatus StopMotor2 (void)    //done//
{
  ErrorStatus status = SUCCESS;

  PWM0_ChannelDutySet(MOTOR2_PWM_CHANNEL, STOP_PWM_VALUE);  
  MOTORB_IN1_Clear();

  
  return status;  
}
