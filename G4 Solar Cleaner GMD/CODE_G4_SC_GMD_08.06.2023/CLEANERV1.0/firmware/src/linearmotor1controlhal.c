#include "main.h"

static stMot1Polarity motor1Polarity;

static void Motor1InitPins(void);
static inline void Motor1Disable (void);
static inline void Motor1Enable (void);

static void Motor1InitPins(void)        //done//
{
  Motor1Disable();
}

void Motor1Init_hal (void)              //done//
{
  motor1Polarity.mot1Polarity = 0;
  Motor1PwmInit();
  Motor1InitPins();
  Motor1Stop();
//  Motor1Enable();
  Motor1Disable();
}

stMot1Polarity * GetSetMot1Polarity (void)      //done//
{
  return &motor1Polarity;
}

void Motor1MoveNegative (void)                                                   
{
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  
  if(motPolarity->mot1Polarity == 0)
  {
    SetMotor1Reverse();
    StartMotor1Reverse();  
  }
  else
  {
    SetMotor1Forward();
    StartMotor1Forward(); 
  }
  Motor1Enable();
}

void Motor1MovePositive (void)                                                   
{
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  
  if(motPolarity->mot1Polarity == 0)
  {
    SetMotor1Forward();
    StartMotor1Forward(); 
  }
  else
  {
    SetMotor1Reverse();
    StartMotor1Reverse();
  }
  Motor1Enable();
}

bool UpdateMotor1PositivePwm (uint16_t pwmDuty)                                                   
{
  bool status = false;
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    status = SetUpdateMotor1ForwardDutyCycle(pwmDuty);
  }
  else
  {
    status = SetUpdateMotor1ReverseDutyCycle(pwmDuty);
  }
  return status;
}

bool UpdateMotor1NegativePwm (float pwmDuty)                                                   
{
  bool status = false;
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    status = SetUpdateMotor1ReverseDutyCycle(pwmDuty);
  }
  else
  {
    status = SetUpdateMotor1ForwardDutyCycle(pwmDuty);
  }
  return status;
}

bool UpdateMotor1Stop (void)                                                   
{
  bool status = true;
  status &= StopMotor1();
  return status;
}

uint8_t GetMotor1PositivePwm (void)                       //done//                                  
{
  uint8_t pwmDuty;
  const stMot2Polarity *motPolarity = GetSetMot2Polarity();
  if(motPolarity->mot2Polarity == 0)
  {
    pwmDuty = GetMotor1ForwardDutyCycle();
  }
  else
  {
    pwmDuty = GetMotor1ReverseDutyCycle();
  }
  return pwmDuty;
}

uint8_t GetMotor1NegativePwm (void)                                                   
{
  uint8_t pwmDuty;
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    pwmDuty = GetMotor1ReverseDutyCycle();
  }
  else
  {
    pwmDuty = GetMotor1ForwardDutyCycle();
  }
  return pwmDuty;
}

void Motor1Break (void)                                                   
{
    Motor1Disable();
    StopMotor1();
}

void Motor1Stop (void)                                            
{
  ErrorStatus status = SUCCESS;
//  StopMotor1(); 
  Motor1Break();
  if(status != SUCCESS)
  {
//    Error_Handler();
  }
}

ErrorStatus SetMotor1Forward (void)     //done//
{
  ErrorStatus status = SUCCESS;
  
  MOTORA_IN1_Clear();
  
  return status;
}

ErrorStatus SetMotor1Reverse (void)     //done//
{
  ErrorStatus status = SUCCESS;
  
  MOTORA_IN1_Set();
  
  return status;
}

static inline void Motor1Disable (void)
{
   MOTORA_PC8_nsleep_Clear(); 
}

static inline void Motor1Enable (void)
{
   MOTORA_PC8_nsleep_Set(); 
}

uint8_t Motor1Fault(void)
{
    if(MOTORA_PC21_lowfault_Get() == 0)
    {
        return 1; 
    }
    else
    {
        return 0;
    }
}


void Motor1CurrentRuntime_inc(void)
{   
    if(motor1A<MOTOR1_CURRENT_LEVEL_1)
    {
        Motor1Runtime_clear();
        motor1_lvl_12_flag=false;
        motor1_lvl_23_flag=false;
        motor1_lvl_34_flag=false;
        motor1_lvl_40_flag=false;
    }
    else if((motor1_lvl_12_flag == false)&&(motor1A>MOTOR1_CURRENT_LEVEL_1)&&(motor1A<MOTOR1_CURRENT_LEVEL_2))
    {
        Motor1Runtime_clear();
        motor1_lvl_12_flag=true;
        motor1_lvl_23_flag=false;
        motor1_lvl_34_flag=false;
        motor1_lvl_40_flag=false;
    }
    else if((motor1_lvl_23_flag == false)&&(motor1A>MOTOR1_CURRENT_LEVEL_2)&&(motor1A<MOTOR1_CURRENT_LEVEL_3))
    {
        Motor1Runtime_clear();
        motor1_lvl_12_flag=false;
        motor1_lvl_23_flag=true;
        motor1_lvl_34_flag=false;
        motor1_lvl_40_flag=false;
    }
    else if((motor1_lvl_34_flag == false)&&(motor1A>MOTOR1_CURRENT_LEVEL_3)&&(motor1A<MOTOR1_CURRENT_LEVEL_4))
    {
        Motor1Runtime_clear();
        motor1_lvl_12_flag=false;
        motor1_lvl_23_flag=false;
        motor1_lvl_34_flag=true;
        motor1_lvl_40_flag=false;
    }
    else if(motor1A>MOTOR1_CURRENT_LEVEL_4)
    {
        Motor1Runtime_clear();
        motor1_lvl_12_flag=false;
        motor1_lvl_23_flag=false;
        motor1_lvl_34_flag=false;
        motor1_lvl_40_flag=true;
    }
    
    if((motor1_lvl_12_flag == true)||(motor1_lvl_23_flag == true)||(motor1_lvl_34_flag == true))
    {
        if(Motor1RuntimeCurrentTimer < 0xFFFFFFFF)
        {
            Motor1RuntimeCurrentTimer++;
        }
        else
        {
            Motor1RuntimeCurrentTimer = 0;
        }
    }
}

uint32_t Motor1Runtime_get(void)
{
    return Motor1RuntimeCurrentTimer;
}

void Motor1Runtime_clear(void)
{
    Motor1RuntimeCurrentTimer = 0;
}