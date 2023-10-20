#include "main.h"

static stMot1Polarity motor1Polarity;
static void Motor1InitPins(void);

void MotorDriveOff(void)
{
  STEPPERMOTOR1_EN_Set();
}

void MotorDriveOn(void)
{
  STEPPERMOTOR1_EN_Clear();
}

void Motor1Enable (void)
{
  STEPPERMOTOR1_EN_Set();
}

void Motor1Disable (void)
{
  STEPPERMOTOR1_EN_Clear();
}

void MotorPositive(void)
{
  STEPPERMOTOR1_DIR_Clear();
}

void MotorNegative(void)
{
  STEPPERMOTOR1_DIR_Set();
}

static void Motor1InitPins(void)                                                   //done//
{
    Motor1Disable();
} 

void Motor1Init_hal (void)
{
    motor1Polarity.mot1Polarity = 0;
    Motor1PwmInit();
    Motor1InitPins();
    Motor1Stop();
    Motor1Enable();
}

stMot1Polarity * GetSetMot1Polarity (void)                                       //done//
{
    return &motor1Polarity;
}

void Motor1MoveNegative (void)                                                   
{
    Motor1MoveNegative_stepper();
}

void Motor1MoveNegative_stepper(void)
{
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    MotorNegative();
    StartPulse();
  }
  else
  {
    MotorPositive();
    StartPulse();
  }
}

void Motor1MovePositive (void)                                                   
{
    Motor1MovePositive_stepper();
}

void Motor1MovePositive_stepper(void)
{
    const stMot1Polarity *motPolarity = GetSetMot1Polarity();
    if(motPolarity->mot1Polarity == 0)
    {
        MotorPositive();
        StartPulse();
    }
    else
    {
        MotorNegative();
        StartPulse();
  }
}

bool UpdateMotor1PositivePwm (uint16_t pwmDuty)                                                   
{
    return(UpdateMotor1PositiveFrequency(pwmDuty));
}

bool UpdateMotor1PositiveFrequency(uint16_t pwmDuty)
{
    bool status = false;
    const stMot1Polarity *motPolarity = GetSetMot1Polarity();
    if(motPolarity->mot1Polarity == 0)
    {
    MotorPositive();
    status = UpdatePulseFrequencyAndRun((uint32_t)pwmDuty);
    }
    else
    {
    MotorNegative();
    status = UpdatePulseFrequencyAndRun((uint32_t)pwmDuty);
    }
    return status;
}

bool UpdateMotor1NegativePwm (uint16_t pwmDuty)                                                   
{
    return(UpdateMotor1NegativeFrequency(pwmDuty));
}

bool UpdateMotor1NegativeFrequency(uint16_t pwmDuty)
{
  bool status = false;
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    MotorNegative();
    status = UpdatePulseFrequencyAndRun((uint32_t)pwmDuty);
  }
  else
  {
    MotorPositive();
    status = UpdatePulseFrequencyAndRun((uint32_t)pwmDuty);
  }
  return status;
}

bool UpdateMotor1StopPwm (uint16_t pwmDuty)                                                   
{
    return (UpdateMotor1StopFrequency(pwmDuty));
}

bool UpdateMotor1StopFrequency(uint16_t pwmDuty)
{
    bool status = true;
    status &= StopPulse();
    return status;
}
uint32_t GetMotor1PositivePwm (void)                                                   
{
    return(GetMotorFrequency());
}

uint32_t GetMotorFrequency(void)
{
    uint32_t pwmDuty;
    const stMot1Polarity *motPolarity = GetSetMot1Polarity();
    if(motPolarity->mot1Polarity == 0)
    {
        pwmDuty = GetPulseFrequency();
    }
    else
    {
        pwmDuty = GetPulseFrequency();
    }
    return pwmDuty;
}

uint32_t GetMotor1NegativePwm (void)                                                   
{
    return(GetMotorFrequency());
}

void Motor1Break (void)                                                   
{
    Motor1Break_stepper();
}

void Motor1Break_stepper(void)
{
    Motor1Disable();
    StopPulse();
}

void Motor1Stop (void)                                            
{
    Motor1Stop_stepper();
}

void Motor1Stop_stepper(void)
{
    ErrorStatus status = SUCCESS;

    StopPulse();
    if(status != SUCCESS)
    {
//        Error_Handler();
    }
}





