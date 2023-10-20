#include "main.h"

#define DEFAULT_BRUSHMOTOR_FORWARD_DUTY_CYCLE             0
#define DEFAULT_BRUSHmotor_REVERSE_DUTY_CYCLE             0

static uint8_t BrushMotorForwardDutyCycle = DEFAULT_BRUSHMOTOR_FORWARD_DUTY_CYCLE;
static uint8_t BrushMotorReverseDutyCycle = DEFAULT_BRUSHmotor_REVERSE_DUTY_CYCLE;

bool BrushPwmInit(void)             //done//
{  
  PWM0_ChannelsStart(PWM_CHANNEL_3_MASK);
    
  if(SetBrushMotorForwardDutyCycle(BrushMotorForwardDutyCycle) != SUCCESS)
  {
//    Error_Handler();
    return false;
  }
  if(SetBrushMotorReverseDutyCycle(BrushMotorReverseDutyCycle) != SUCCESS)
  {
//    Error_Handler();
    return false;
  }
  
  return true;
}

ErrorStatus SetBrushMotorForwardDutyCycle (uint8_t dutyCycle)      //done//   //SetBrushPwmCh1DutyCycle
{
  ErrorStatus status = SUCCESS;
  int16_t duty = (int16_t)(dutyCycle);
  if(duty >= 0 && duty <= 100)
  {
    BrushMotorForwardDutyCycle = (uint8_t)duty;
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }
  return status;
}

ErrorStatus SetBrushMotorReverseDutyCycle (uint8_t dutyCycle)      //done//  //SetBrushPwmCh1DutyCycle
{
  ErrorStatus status = SUCCESS;
  int16_t duty = (int16_t)(dutyCycle);
  if(duty >= 0 && duty <= 100)
  {
    BrushMotorReverseDutyCycle = (uint8_t)duty;
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }
  return status;
}

bool SetUpdateBrushMotorForwardDutyCycle (uint8_t dutyCycle)        //done//                           //bool SetUpdateBrushPwmCh1DutyCycle (float dutyCycle)
{
  ErrorStatus status = SUCCESS;
  status = SetBrushMotorForwardDutyCycle(dutyCycle);
  if(status == SUCCESS)
  {
    StartBrushMotorForward();
  }
  return (status == SUCCESS);
}

bool SetUpdateBrushMotorReverseDutyCycle (uint8_t dutyCycle)        //done//                           //bool SetUpdateBrushPwmCh1DutyCycle (float dutyCycle)
{
  ErrorStatus status = SUCCESS;
  status = SetBrushMotorReverseDutyCycle(dutyCycle);
  if(status == SUCCESS)
  {
    StartBrushMotorReverse();
  }
  return (status == SUCCESS);
}

uint8_t GetBrushMotorForwardDutyCycle (void)     //done//
{
  return BrushMotorForwardDutyCycle;
}

uint8_t GetBrushMotorReverseDutyCycle (void)     //done//
{
  return BrushMotorReverseDutyCycle;
}

ErrorStatus StartBrushMotorForward (void)    //done//
{
  ErrorStatus status = SUCCESS;
  
  PWM0_ChannelDutySet(BRUSH_PWM_CHANNEL, (40000*BrushMotorForwardDutyCycle)/100);
  
  return status;  
}

ErrorStatus StartBrushMotorReverse (void)    //done//
{
  ErrorStatus status = SUCCESS;
  
//  PWM0_ChannelDutySet(BRUSH_PWM_CHANNEL, 40000 -((40000*BrushMotorReverseDutyCycle)/100));
  PWM0_ChannelDutySet(BRUSH_PWM_CHANNEL, (40000*BrushMotorReverseDutyCycle)/100);
  
  return status;  
}

ErrorStatus StopBrushMotor (void)                 //done//
{
  ErrorStatus status = SUCCESS;
  
  PWM0_ChannelDutySet(BRUSH_PWM_CHANNEL, STOP_PWM_VALUE);
  MOTORC_IN1_Clear();
  MOTORC_PC14_nsleep_Clear();
  
  return status;  
}
