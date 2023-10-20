#include "main.h"

static stBrushMotPolarity brushMotorPolarity;
static void BrushMotorInitPins(void);
static inline void BrushMotorEnable (void);
static inline void BrushMotorDisable (void);

static void BrushMotorInitPins(void)                //done//
{
//  BrushMotorDisable();
    BrushMotorStop();
} 

void BrushMotorInit_hal (void)                  //done//
{
    brushMotorPolarity.mot1Polarity = 0;                                                          //0 -> forward | 1-> reverse | 2 -> stop
    BrushPwmInit();
    BrushMotorInitPins();
//    BrushMotorStop();
//    BrushMotorEnable();
//    BrushMotorDisable();
}

stBrushMotPolarity * GetSetBrushMotPolarity (void)  //done//
{
  return &brushMotorPolarity;
}

void BrushMotorMoveNegative (void)                  //done//                                               
{
  const stBrushMotPolarity *motPolarity = GetSetBrushMotPolarity();
  
  if(motPolarity->mot1Polarity == 0)
  {
    SetBrushMotorReverse();
    StartBrushMotorReverse();                                                                                      //*
  }
  else
  {
    SetBrushMotorForward();
    StartBrushMotorForward(); 
  }
//  BrushMotorEnable();
  MOTORC_IN1_Clear();
  MOTORC_PC14_nsleep_Set();
}

void BrushMotorMovePositive (void)               //done//                                           
{
  const stBrushMotPolarity *motPolarity = GetSetBrushMotPolarity();
  
  if(motPolarity->mot1Polarity == 0)
  {
    SetBrushMotorForward();
    StartBrushMotorForward(); 
  }
  else
  {
    SetBrushMotorReverse();
    StartBrushMotorReverse();
  }
//  BrushMotorEnable();
  MOTORC_IN1_Set();
  MOTORC_PC14_nsleep_Clear();
}

bool UpdateBrushMotorPositivePwm (uint8_t pwmDuty)               //done//                                     
{
  bool status = false;
  const stBrushMotPolarity *motPolarity = GetSetBrushMotPolarity();
  if(motPolarity->mot1Polarity == 0)
  {
    status = SetUpdateBrushMotorForwardDutyCycle(pwmDuty);
  }
  else
  {
    status = SetUpdateBrushMotorReverseDutyCycle(pwmDuty);          //done// 
  }
  return status;
}

bool UpdateBrushMotorNegativePwm (uint8_t pwmDuty)              //done//                                      
{
  bool status = false;
  const stBrushMotPolarity *motPolarity = GetSetBrushMotPolarity();
  if(motPolarity->mot1Polarity == 0)
  {
    status = SetUpdateBrushMotorReverseDutyCycle(pwmDuty);
  }
  else
  {
    status = SetUpdateBrushMotorForwardDutyCycle(pwmDuty);
  }
  return status;
}

bool UpdateBrushMotorStopPwm (void)                           //done//                         
{
  bool status = true;
  status &= StopMotor1();
  return status;
}

uint8_t GetBrushMotorPositivePwm (void)           //done//                                        
{
  uint8_t pwmDuty;
  const stBrushMotPolarity *motPolarity = GetSetBrushMotPolarity();
  if(motPolarity->mot1Polarity == 0)
  {
    pwmDuty = GetBrushMotorForwardDutyCycle();
  }
  else
  {
    pwmDuty = GetBrushMotorReverseDutyCycle();
  }
  return pwmDuty;
}

uint8_t GetBrushMotorNegativePwm (void)         //done//                                          
{
  uint8_t pwmDuty;
  const stBrushMotPolarity *motPolarity = GetSetBrushMotPolarity();
  if(motPolarity->mot1Polarity == 0)
  {
    pwmDuty = GetBrushMotorReverseDutyCycle();
  }
  else
  {
    pwmDuty = GetBrushMotorForwardDutyCycle();
  }
  return pwmDuty;
}

void BrushMotorBreak (void)                     //done//                              
{
//    BrushMotorDisable();
//    StopBrushMotor();
    
    MOTORC_IN1_Clear();
    MOTORC_PC14_nsleep_Clear();
}

void BrushMotorStop (void)                     //done//                       
{
  ErrorStatus status = SUCCESS;
  BrushMotorBreak();
//  StopBrushMotor(); 
  if(status != SUCCESS)
  {
//    Error_Handler();
  }
}

ErrorStatus SetBrushMotorForward (void)     //done//
{
  ErrorStatus status = SUCCESS;
  
  MOTORC_IN1_Clear();
  
  return status;
}

ErrorStatus SetBrushMotorReverse (void)     //done//
{
  ErrorStatus status = SUCCESS;
  
  MOTORC_IN1_Set();
  
  return status;
}

static inline void BrushMotorDisable (void) //done//
{
   MOTORC_PC14_nsleep_Clear();
}

static inline void BrushMotorEnable (void)  //done//
{
   MOTORC_PC14_nsleep_Set();
}

uint8_t BrushMotorFault(void)
{
    if(MOTORC_PD14_lowfault_Get() == 0)
    {
        return 1; 
    }
    else
    {
        return 0;
    }
}

void Motor3CurrentRuntime_inc(void)
{   
    if(motor3A<MOTOR3_CURRENT_LEVEL_1)
    {
        Motor3Runtime_clear();
        motor3_lvl_12_flag=false;
        motor3_lvl_23_flag=false;
        motor3_lvl_34_flag=false;
        motor3_lvl_40_flag=false;
    }
    else if((motor3_lvl_12_flag == false)&&(motor3A>MOTOR3_CURRENT_LEVEL_1)&&(motor3A<MOTOR3_CURRENT_LEVEL_2))
    {
        Motor3Runtime_clear();
        motor3_lvl_12_flag=true;
        motor3_lvl_23_flag=false;
        motor3_lvl_34_flag=false;
        motor3_lvl_40_flag=false;
    }
    else if((motor3_lvl_23_flag == false)&&(motor3A>MOTOR3_CURRENT_LEVEL_2)&&(motor3A<MOTOR3_CURRENT_LEVEL_3))
    {
        Motor3Runtime_clear();
        motor3_lvl_12_flag=false;
        motor3_lvl_23_flag=true;
        motor3_lvl_34_flag=false;
        motor3_lvl_40_flag=false;
    }
    else if((motor3_lvl_34_flag == false)&&(motor3A>MOTOR3_CURRENT_LEVEL_3)&&(motor3A<MOTOR3_CURRENT_LEVEL_4))
    {
        Motor3Runtime_clear();
        motor3_lvl_12_flag=false;
        motor3_lvl_23_flag=false;
        motor3_lvl_34_flag=true;
        motor3_lvl_40_flag=false;
    }
    else if(motor3A>MOTOR3_CURRENT_LEVEL_4)
    {
        Motor3Runtime_clear();
        motor3_lvl_12_flag=false;
        motor3_lvl_23_flag=false;
        motor3_lvl_34_flag=false;
        motor3_lvl_40_flag=true;
    }
    
    if((motor3_lvl_12_flag == true)||(motor3_lvl_23_flag == true)||(motor3_lvl_34_flag == true))
    {
        if(Motor3RuntimeCurrentTimer < 0xFFFFFFFF)
        {
            Motor3RuntimeCurrentTimer++;
        }
        else
        {
            Motor3RuntimeCurrentTimer = 0;
        }
    }
}

uint32_t Motor3Runtime_get(void)
{
    return Motor3RuntimeCurrentTimer;
}

void Motor3Runtime_clear(void)
{
    Motor3RuntimeCurrentTimer = 0;
}